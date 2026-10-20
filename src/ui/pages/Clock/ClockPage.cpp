#include "ui/pages/Clock/ClockPage.h"
#include "Config.h"
#include "ui/TopBar.h"
#include "ui/App.h"

// ---------------------------------------------------------------------------
// Layout, 480x320 landscape
// ---------------------------------------------------------------------------
static const int16_t SCR_W = 480;
static const int16_t CX = SCR_W / 2;
 
static const int16_t DATE_Y = 62;                  // vertical centre of the date
static const int16_t TIME_BASELINE = 186;          // baseline of the big HH:MM
 
static const int16_t BAR_X = 120, BAR_Y = 204, BAR_W = 240, BAR_H = 5;
static const int16_t SEC_X = 372;                  // seconds number, left edge
 
static const int16_t PILL_W = 172, PILL_H = 52;
static const int16_t PILL_X = CX - PILL_W / 2, PILL_Y = 240;
static const int16_t PILL_CY = PILL_Y + PILL_H / 2;
static const int16_t THERMO_X = PILL_X + 30;       // thermometer centre line
static const int16_t TEMP_X = PILL_X + 52;         // temperature value, left edge
 
// ---

void ClockPage::drawStaticLayout()
{
    tft_.fillScreen(Theme::Background);
    TopBar topBar(tft_);
    topBar.draw(PAGE_CLOCK, true,"TODO");
    drawClockTemperature("22.4");
}


void ClockPage::drawThermometer() {
  const uint16_t bg = Theme::Panel;
  const int16_t top = PILL_Y + 10, stemH = 22;
  tft_.fillSmoothRoundRect(THERMO_X - 6, top, 13, stemH + 6, 6, Theme::Dim, bg);    // stem
  tft_.fillSmoothCircle(THERMO_X, top + stemH + 4, 10, Theme::Dim, bg);              // bulb
  tft_.fillRect(THERMO_X - 2, top + 8, 5, stemH - 4, Theme::Accent);                 // liquid
  tft_.fillSmoothCircle(THERMO_X, top + stemH + 4, 6, Theme::Accent, Theme::Dim);
}
 
void ClockPage::drawClockTemperature(const char *value) {
  const uint16_t bg = Theme::Panel;
  tft_.fillSmoothRoundRect(PILL_X, PILL_Y, PILL_W, PILL_H, PILL_H / 2, bg, Theme::Background);
  drawThermometer();
 
  // Value
  tft_.loadFont(FontTemp);
  tft_.setTextDatum(L_BASELINE);
  tft_.setTextColor(Theme::Text, bg);
  const int16_t baseline = PILL_CY + 10;
  tft_.drawString(value, TEMP_X, baseline);
  int16_t x = TEMP_X + tft_.textWidth(value);
  tft_.unloadFont();
 
  // Degree ring + unit
  tft_.fillSmoothCircle(x + 7, baseline - 18, 4, Theme::Text, bg);
  tft_.fillSmoothCircle(x + 7, baseline - 18, 2, bg, Theme::Text);
  tft_.loadFont(FontSmall);
  tft_.setTextDatum(L_BASELINE);
  tft_.setTextColor(Theme::Text, bg);
  tft_.drawString("C", x + 13, baseline - 6);
  tft_.unloadFont();
}


void ClockPage::update()
{
    static uint32_t lastTick = 0;
    if (millis() - lastTick >= 1000)
    {
        lastTick = millis();
        datetime_t dt = App::instance().dateTime();
        drawTime(dt.hour, dt.min, dt.sec);
        drawDate(dt.year, dt.month, dt.day, dt.dotw);
    }
}

void ClockPage::drawTime(uint8_t hour,  uint8_t min,  uint8_t sec)
{

    char text[9];
    snprintf(text, sizeof(text), "%02d:%02d:%02d", hour, min, sec);

    tft_.setTextColor(Theme::Text);
    tft_.setFreeFont(&FreeSansBold24pt7b);
    tft_.fillRect(160,150, 300, 50, Theme::Background);
    tft_.drawString(text, 170, 150);
}

void ClockPage::drawDate(uint16_t year, uint8_t month, uint8_t day, uint8_t dotweek)
{
    static const char* months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    char buf[32];
    snprintf(buf, sizeof(buf), "%s, %s %d",
             dotwToString(dotweek),
             months[month - 1],  // month is 1-based
             day);

    tft_.setTextColor(Theme::Text);
    tft_.setFreeFont(&FreeSansBold12pt7b);
    tft_.setTextDatum(MC_DATUM);
    tft_.fillRect(100, 10, 200, 120, Theme::Background);
    tft_.drawString(buf, 240, 20);
}

const char* ClockPage::dotwToString(uint8_t dotw) {
    switch (dotw) {
        case 0: return "Sunday";
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";
        default: return "Unknown";
    }
}