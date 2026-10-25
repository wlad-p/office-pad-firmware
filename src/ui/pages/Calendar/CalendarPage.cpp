#include "CalendarPage.h"
#include "ui/TopBar.h"
#include "ui/App.h"
#include "fonts/FontDate.h"
#include "fonts/FontSmall.h"
#include "fonts/FontAlbum.h"

// ---------------------------------------------------------------------------
// Layout, 480x320 landscape
// ---------------------------------------------------------------------------
static const int16_t GRID_X = 24, COL_W = 32;              // 7 columns, Monday first
static const int16_t MONTH_Y = 62;                         // month title, vertical centre
static const int16_t WEEKDAY_Y = 92;
static const int16_t ROW0_Y = 118, ROW_H = 33;             // day number centres
static const int16_t TODAY_R = 14;

static const int16_t CARD_X = 266, CARD_Y = 46, CARD_W = 190, CARD_H = 258;
static const int16_t CARD_PAD = 16;
static const int16_t EVENT_H = 44;
static const int16_t HEADING_H = 26;

static const uint16_t FADED = rgb565(166, 140, 104);       // past days
static const uint16_t EVENT_COLORS[] = {Theme::Accent, rgb565(106, 132, 96), rgb565(98, 110, 160)};

static const char *const MONTHS[] = {"January", "February", "March", "April", "May", "June", "July",
                                     "August", "September", "October", "November", "December"};

// ---------------------------------------------------------------------------
// Date helpers: day numbers (days since 1970-01-01) make "today", "tomorrow" and ranges easy
// ---------------------------------------------------------------------------
static int32_t dayNumber(int32_t y, uint32_t m, uint32_t d)
{
    y -= m <= 2;
    const int32_t era = (y >= 0 ? y : y - 399) / 400;
    const uint32_t yoe = (uint32_t)(y - era * 400);
    const uint32_t doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
    const uint32_t doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + (int32_t)doe - 719468;
}

static uint8_t daysInMonth(uint16_t y, uint8_t m)
{
    return (uint8_t)(dayNumber(m == 12 ? y + 1 : y, m == 12 ? 1 : m + 1, 1) - dayNumber(y, m, 1));
}

static uint8_t mondayIndex(int32_t day) { return (uint8_t)((day + 3) % 7); }   // Monday = 0

// First and last day an event covers. An end at exactly 00:00 is exclusive (how most calendars send all-day events).
static void eventDays(const CalendarEvent &e, int32_t &first, int32_t &last)
{
    first = dayNumber(e.startYear, e.startMonth, e.startDay);
    last = e.endYear ? dayNumber(e.endYear, e.endMonth, e.endDay) : first;
    if (last > first && e.endHour == 0 && e.endMin == 0)
        last--;
    if (last < first)
        last = first;
}

static bool sameEvent(const CalendarEvent &a, const CalendarEvent &b)
{
    return strcmp(a.title, b.title) == 0 && strcmp(a.location, b.location) == 0 &&
           a.startYear == b.startYear && a.startMonth == b.startMonth && a.startDay == b.startDay &&
           a.startHour == b.startHour && a.startMin == b.startMin &&
           a.endYear == b.endYear && a.endMonth == b.endMonth && a.endDay == b.endDay &&
           a.endHour == b.endHour && a.endMin == b.endMin && a.allDay == b.allDay;
}

static bool coversDay(const CalendarEvent &e, int32_t day)
{
    int32_t first, last;
    eventDays(e, first, last);
    return day >= first && day <= last;
}

// Draws text shortened with "…" so it fits maxW. Font must be loaded.
static void drawFitted(TFT_eSPI &tft, const char *text, int16_t x, int16_t y, int16_t maxW)
{
    char buf[64];
    strncpy(buf, text ? text : "", sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    if (tft.textWidth(buf) > maxW)
    {
        const char *ell = "\xE2\x80\xA6";
        const int16_t ellW = tft.textWidth(ell);
        size_t len = strlen(buf);
        while (len > 0)
        {
            do { len--; } while (len > 0 && (buf[len] & 0xC0) == 0x80);   // one UTF-8 character back
            buf[len] = '\0';
            if (tft.textWidth(buf) + ellW <= maxW) break;
        }
        while (len > 0 && buf[len - 1] == ' ') buf[--len] = '\0';
        strncat(buf, ell, sizeof(buf) - strlen(buf) - 1);
    }
    tft.drawString(buf, x, y);
}

// ---------------------------------------------------------------------------

void CalendarPage::drawStaticLayout()
{
    tft_.fillScreen(Theme::Background);
    lastMinute_ = -1;
    shownDay_ = -1;
    update();
}

void CalendarPage::update()
{
    datetime_t dt = App::instance().dateTime();
    if (dt.month < 1 || dt.month > 12)
        return;   // clock not synced yet

    if (dt.min != lastMinute_)
    {
        lastMinute_ = dt.min;
        char hm[6];
        snprintf(hm, sizeof(hm), "%02u:%02u", (unsigned)dt.hour, (unsigned)dt.min);
        TopBar topBar(tft_);
        topBar.draw(PAGE_CALENDAR, true, hm);
    }

    int32_t today = dayNumber(dt.year, dt.month, dt.day);
    if (today != shownDay_)
    {
        drawMonth(today);
        drawAgenda(today);
        shownDay_ = today;
    }
}

void CalendarPage::updateEvents(const CalendarEvent *events, size_t count)
{
    if (count > MAX_EVENTS)
        count = MAX_EVENTS;

    // Skip the redraw when the PC sends the same events again
    bool same = count == eventCount_;
    for (size_t i = 0; same && i < count; i++)
        same = sameEvent(events[i], events_[i]);
    if (same)
        return;

    for (size_t i = 0; i < count; i++)
        events_[i] = events[i];
    eventCount_ = count;

    if (shownDay_ >= 0)
    {
        drawMonth(shownDay_);   // event dots
        drawAgenda(shownDay_);
    }
}

bool CalendarPage::hasEventOn(int32_t day) const
{
    for (size_t i = 0; i < eventCount_; i++)
        if (coversDay(events_[i], day))
            return true;
    return false;
}

// ---------------------------------------------------------------------------
// Month grid
// ---------------------------------------------------------------------------
void CalendarPage::drawMonth(int32_t today)
{
    datetime_t dt = App::instance().dateTime();
    const uint16_t year = dt.year;
    const uint8_t month = dt.month;
    const int32_t first = dayNumber(year, month, 1);
    const uint8_t offset = mondayIndex(first);
    const uint8_t days = daysInMonth(year, month);
    const uint16_t bg = Theme::Background;

    tft_.fillRect(0, MONTH_Y - 16, CARD_X - 4, 320 - (MONTH_Y - 16), bg);

    // "October 2026"
    tft_.loadFont(FontDate);
    tft_.setTextDatum(ML_DATUM);
    tft_.setTextColor(Theme::Text, bg);
    tft_.drawString(MONTHS[month - 1], GRID_X + 4, MONTH_Y);
    int16_t x = GRID_X + 4 + tft_.textWidth(MONTHS[month - 1]) + 8;
    tft_.setTextColor(Theme::SubText, bg);
    char yr[6];
    snprintf(yr, sizeof(yr), "%u", year);
    tft_.drawString(yr, x, MONTH_Y);
    tft_.unloadFont();

    // M T W T F S S
    tft_.loadFont(FontSmall);
    tft_.setTextDatum(MC_DATUM);
    const char *const initials[] = {"M", "T", "W", "T", "F", "S", "S"};
    for (uint8_t c = 0; c < 7; c++)
    {
        tft_.setTextColor(c >= 5 ? Theme::Accent : Theme::SubText, bg);
        tft_.drawString(initials[c], GRID_X + c * COL_W + COL_W / 2, WEEKDAY_Y);
    }
    tft_.unloadFont();

    // Days
    tft_.loadFont(FontAlbum);
    tft_.setTextDatum(MC_DATUM);
    for (uint8_t d = 1; d <= days; d++)
    {
        uint8_t cell = offset + d - 1;
        int16_t cx = GRID_X + (cell % 7) * COL_W + COL_W / 2;
        int16_t cy = ROW0_Y + (cell / 7) * ROW_H;
        int32_t day = first + d - 1;
        char num[4];
        snprintf(num, sizeof(num), "%u", d);

        if (day == today)
        {
            tft_.unloadFont();
            tft_.fillSmoothCircle(cx, cy, TODAY_R, Theme::Accent, bg);
            tft_.loadFont(FontSmall);
            tft_.setTextColor(bg, Theme::Accent);
            tft_.drawString(num, cx, cy);
            tft_.unloadFont();
            tft_.loadFont(FontAlbum);
            continue;
        }

        tft_.setTextColor(day < today ? FADED : Theme::Text, bg);
        tft_.drawString(num, cx, cy);
        if (hasEventOn(day))
            tft_.fillSmoothCircle(cx, cy + 12, 2, day < today ? FADED : Theme::Accent, bg);
    }
    tft_.unloadFont();
}

// ---------------------------------------------------------------------------
// Agenda card: today, then tomorrow
// ---------------------------------------------------------------------------
int16_t CalendarPage::drawEvent(const CalendarEvent &e, uint8_t index, int16_t y)
{
    const uint16_t bg = Theme::Panel;
    const int16_t x = CARD_X + CARD_PAD;
    const int16_t textX = x + 14, textW = CARD_W - CARD_PAD * 2 - 14;

    tft_.fillSmoothRoundRect(x, y + 4, 4, EVENT_H - 14, 2, EVENT_COLORS[index % 3], bg);

    tft_.loadFont(FontSmall);
    tft_.setTextDatum(TL_DATUM);
    tft_.setTextColor(Theme::Text, bg);
    drawFitted(tft_, e.title, textX, y + 2, textW);
    tft_.unloadFont();

    char when[24];
    if (e.allDay)
        strcpy(when, "All day");
    else
        snprintf(when, sizeof(when), "%02u:%02u - %02u:%02u", e.startHour, e.startMin, e.endHour, e.endMin);
    tft_.loadFont(FontAlbum);
    tft_.setTextColor(Theme::SubText, bg);
    drawFitted(tft_, when, textX, y + 20, textW);
    tft_.unloadFont();

    return y + EVENT_H;
}

void CalendarPage::drawAgenda(int32_t today)
{
    const uint16_t bg = Theme::Panel;
    const int16_t x = CARD_X + CARD_PAD;
    const int16_t bottom = CARD_Y + CARD_H - CARD_PAD;
    tft_.fillSmoothRoundRect(CARD_X, CARD_Y, CARD_W, CARD_H, 14, bg, Theme::Background);

    int16_t y = CARD_Y + CARD_PAD;
    uint8_t colour = 0;

    for (uint8_t section = 0; section < 2; section++)
    {
        const int32_t day = today + section;

        // Collect this day's events: all-day first, then by start time
        const CalendarEvent *list[MAX_EVENTS];
        uint8_t n = 0;
        for (size_t i = 0; i < eventCount_; i++)
            if (coversDay(events_[i], day))
                list[n++] = &events_[i];
        for (uint8_t i = 1; i < n; i++)
            for (uint8_t j = i; j > 0; j--)
            {
                const CalendarEvent *a = list[j - 1], *b = list[j];
                int ka = a->allDay ? -1 : a->startHour * 60 + a->startMin;
                int kb = b->allDay ? -1 : b->startHour * 60 + b->startMin;
                if (ka <= kb) break;
                list[j - 1] = b;
                list[j] = a;
            }

        if (section == 1)
        {
            if (y + 12 + HEADING_H + EVENT_H > bottom)
                break;   // no room for tomorrow
            tft_.drawFastHLine(x, y + 4, CARD_W - CARD_PAD * 2, Theme::Dim);
            y += 14;
        }

        tft_.loadFont(FontSmall);
        tft_.setTextDatum(TL_DATUM);
        tft_.setTextColor(section == 0 ? Theme::Accent : Theme::SubText, bg);
        tft_.drawString(section == 0 ? "TODAY" : "TOMORROW", x, y);
        tft_.unloadFont();
        y += HEADING_H;

        if (n == 0)
        {
            tft_.loadFont(FontAlbum);
            tft_.setTextColor(Theme::SubText, bg);
            tft_.drawString("No events", x, y);
            tft_.unloadFont();
            y += 26;
            continue;
        }

        for (uint8_t i = 0; i < n; i++)
        {
            // Keep space for a "+ N more" line unless this is the last event
            bool last = i == n - 1;
            bool fits = y + EVENT_H + (last ? 0 : 22) <= bottom;
            if (!fits)
            {
                char more[16];
                snprintf(more, sizeof(more), "+ %u more", (unsigned)(n - i));
                tft_.loadFont(FontAlbum);
                tft_.setTextColor(Theme::SubText, bg);
                tft_.drawString(more, x, y);
                tft_.unloadFont();
                y += 22;
                break;
            }
            y = drawEvent(*list[i], colour++, y);
        }
    }
}
