// MediaPage.cpp
#include "MediaPage.h"
#include "ui/pages/Media/LayoutConfig.h"
#include "ui/TopBar.h"
#include "ui/App.h"
#include "Config.h"
static const char ELLIPSIS[] = "\xE2\x80\xA6";   // "…" in UTF-8

void MediaPage::drawStaticLayout()
{
    Serial.println("{\"dbg\":\"static\"}");


    tft_.fillScreen(Theme::Background);
    lastBarWidth_ = -1;
    lastMinute_ = -1;
    TopBar topBar(tft_);
    topBar.draw(PAGE_MEDIA, true, "TODO");

    tft_.loadFont(FontSmall);
    tft_.setTextDatum(TL_DATUM);
    tft_.setTextColor(Theme::Accent, Theme::Background, true);
    tft_.drawString("NOW PLAYING", UI::contentX, UI::labelY);
    tft_.unloadFont();

    drawAlbumArt(UI::artX, UI::artY, UI::artSize, UI::artSize);
    updateProgress(0, 0);
}

// void MediaPage::update()
// {
//     datetime_t dt = App::instance().dateTime();
//     if (dt.min == lastMinute_) return;
//     lastMinute_ = dt.min;

//     char hm[6];
//     snprintf(hm, sizeof(hm), "%02u:%02u", (unsigned)dt.hour, (unsigned)dt.min);
//     TopBar topBar(tft_);
//     topBar.draw(PAGE_MEDIA, true, hm);
// }

// Draws text at (x, y) top-left, shortened with "…" if wider than maxW.
// The padding fills the rest of the line with the background, so old text disappears without flicker.
void MediaPage::drawFitted(const char *text, int16_t x, int16_t y, int16_t maxW)
{
    char buf[96];
    strncpy(buf, text ? text : "", sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    if (tft_.textWidth(buf) > maxW) {
        const int16_t ellW = tft_.textWidth(ELLIPSIS);
        size_t len = strlen(buf);
        while (len > 0) {
            do { len--; } while (len > 0 && (buf[len] & 0xC0) == 0x80);   // step back one UTF-8 character
            buf[len] = '\0';
            if (tft_.textWidth(buf) + ellW <= maxW) break;
        }
        while (len > 0 && buf[len - 1] == ' ') buf[--len] = '\0';
        strncat(buf, ELLIPSIS, sizeof(buf) - strlen(buf) - 1);
    }

    tft_.setTextDatum(TL_DATUM);
    tft_.setTextPadding(maxW);
    tft_.drawString(buf, x, y);
    tft_.setTextPadding(0);
}

void MediaPage::updateTrack(const char *title, const char *artist, const char *album)
{
    // Title
    tft_.loadFont(FontTitle);
    tft_.setTextColor(Theme::Text, Theme::Background, true);
    drawFitted(title, UI::contentX, UI::titleY, UI::contentW);
    tft_.unloadFont();

    // Artist
    tft_.loadFont(FontArtist);
    tft_.setTextColor(Theme::SubText, Theme::Background, true);
    drawFitted(artist, UI::contentX, UI::artistY, UI::contentW);
    tft_.unloadFont();

    // Album (an empty string just clears the line)
    tft_.loadFont(FontAlbum);
    tft_.setTextColor(Theme::SubText, Theme::Background, true);
    drawFitted(album, UI::contentX, UI::albumY, UI::contentW);
    tft_.unloadFont();
}

void MediaPage::clearMetadataArea()
{
    tft_.fillRect(UI::contentX, UI::titleY, UI::contentW, UI::albumY + 20 - UI::titleY, Theme::Background);
}

void MediaPage::updateProgress(int currentSec, int totalSec)
{
    if (totalSec < 0) totalSec = 0;
    if (currentSec < 0) currentSec = 0;
    if (totalSec > 0 && currentSec > totalSec) currentSec = totalSec;

    int16_t width = totalSec > 0 ? (int32_t)UI::barWidth * currentSec / totalSec : 0;

    // Bar: only paint what changed, redraw fully when it shrinks (new track, seeking back)
    if (lastBarWidth_ < 0 || width < lastBarWidth_) {
        tft_.fillSmoothRoundRect(UI::barX, UI::barY, UI::barWidth, UI::barHeight, UI::barHeight / 2, Theme::Panel, Theme::Background);
        if (width >= UI::barHeight)
            tft_.fillSmoothRoundRect(UI::barX, UI::barY, width, UI::barHeight, UI::barHeight / 2, Theme::Accent, Theme::Background);
        else if (width > 0)
            tft_.fillRect(UI::barX, UI::barY, width, UI::barHeight, Theme::Accent);
    } else if (width >= UI::barWidth - UI::barHeight / 2) {
        // Near the end: full rounded bar so the right end matches the track
        tft_.fillSmoothRoundRect(UI::barX, UI::barY, UI::barWidth, UI::barHeight, UI::barHeight / 2, Theme::Accent, Theme::Background);
    } else if (width > lastBarWidth_) {
        // Only paint the new slice
        tft_.fillRect(UI::barX + lastBarWidth_, UI::barY, width - lastBarWidth_, UI::barHeight, Theme::Accent);
    }
    lastBarWidth_ = width;

    // Times
    char elapsed[16], total[16];
    snprintf(elapsed, sizeof(elapsed), "%d:%02d", currentSec / 60, currentSec % 60);
    snprintf(total, sizeof(total), "%d:%02d", totalSec / 60, totalSec % 60);

    tft_.loadFont(FontAlbum);
    tft_.setTextColor(Theme::SubText, Theme::Background, true);
    const int16_t pad = tft_.textWidth("88:88");
    tft_.setTextPadding(pad);
    tft_.setTextDatum(ML_DATUM);
    tft_.drawString(elapsed, UI::barX, UI::timeY);
    tft_.setTextDatum(MR_DATUM);
    tft_.drawString(total, UI::barX + UI::barWidth, UI::timeY);
    tft_.setTextPadding(0);
    tft_.unloadFont();
}

void MediaPage::drawAlbumArt(int x, int y, int w, int h)
{
    // Placeholder: terracotta tile with a music note, replaced when the art arrives
    tft_.fillSmoothRoundRect(x, y, w, h, 12, Theme::Accent, Theme::Background);

    const uint16_t note = Theme::Background, bg = Theme::Accent;
    const int16_t cx = x + w / 2, cy = y + h / 2;
    tft_.fillSmoothCircle(cx - 12, cy + 18, 10, note, bg);              // note head
    tft_.fillRect(cx - 4, cy - 26, 5, 44, note);                         // stem
    tft_.fillTriangle(cx - 4, cy - 26, cx + 16, cy - 18, cx - 4, cy - 14, note);   // flag
    tft_.fillTriangle(cx + 16, cy - 18, cx + 16, cy - 8, cx - 4, cy - 14, note);
}
