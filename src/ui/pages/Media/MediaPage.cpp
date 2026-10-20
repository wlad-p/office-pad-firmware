// MediaPage.cpp
#include "MediaPage.h"
#include "ui/pages/Media/LayoutConfig.h"
#include "ui/TopBar.h"

void MediaPage::drawStaticLayout()
{
    tft_.fillScreen(Theme::Background);

    TopBar topBar(tft_);
    topBar.draw(PAGE_CLOCK, true,"TODO");

    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM); // Middle Center
    tft_.drawString("NOW PLAYING", UI::artX + UI::artSize + 5, UI::artY);

    // Album Art Frame
    tft_.fillRoundRect(UI::artX, UI::artY, UI::artSize, UI::artSize, 8, TFT_RED);
    tft_.drawRoundRect(UI::artX, UI::artY, UI::artSize, UI::artSize, 8, COLOR_PROGRESS_BG);
    tft_.fillCircle(UI::artX + (UI::artSize / 2), UI::artY + (UI::artSize / 2), 55, Theme::Background);
    tft_.fillCircle(UI::artX + (UI::artSize / 2), UI::artY + (UI::artSize / 2), 18, Theme::Accent);
}

void MediaPage::updateTrack(const char *title, const char *artist, const char *album)
{
    clearMetadataArea();

    // Title
    tft_.setTextColor(Theme::Text);
    tft_.drawString(title, UI::contentX, UI::titleY, 4);

    // Artist
    tft_.setTextColor(Theme::Text);
    tft_.drawString(artist, UI::contentX, UI::artistY, 2);

    // Album
    if (album && strlen(album) > 0)
    {
        tft_.drawString(album, UI::contentX, UI::albumY + 20, 1);
    }
}

void MediaPage::clearMetadataArea()
{
    // Overwrite old text area with the background color before drawing new text
    int width = Layout::screenWidth - UI::contentX;
    tft_.fillRect(UI::contentX, UI::titleY, width, 100, Theme::Background);
}

void MediaPage::updateProgress(int currentSec, int totalSec)
{
    if (totalSec <= 0)
        totalSec = 1; // Prevent division by zero

    float pct = (float)currentSec / (float)totalSec;
    if (pct > 1.0)
        pct = 1.0;
    int currentWidth = UI::barWidth * pct;

    // Redraw Progress Bar
    tft_.fillRect(UI::barX, UI::barY, currentWidth, UI::barHeight, Theme::Accent);
    tft_.fillRect(UI::barX + currentWidth, UI::barY, UI::barWidth - currentWidth, UI::barHeight, COLOR_PROGRESS_BG);

    // Format timestamps
    char elapsed[8];
    char remaining[8];
    sprintf(elapsed, "%d:%02d", currentSec / 60, currentSec % 60);
    sprintf(remaining, "%d:%02d", totalSec / 60, totalSec % 60);

    tft_.setTextColor(Theme::Text, Theme::Background);
    tft_.drawString(elapsed, UI::barX, UI::barY + 12, 2);

    int rightTextWidth = tft_.textWidth(remaining, 2);
    tft_.drawString(remaining, (UI::barX + UI::barWidth) - rightTextWidth, UI::barY + 12, 2);
}

void MediaPage::drawAlbumArt(int x, int y, int w, int h)
{
    tft_.fillRect(x, y, w, h, Theme::Background);
    tft_.drawRect(x, y, w, h, TFT_DARKGREY);                 // outer border
    tft_.drawRect(x + 1, y + 1, w - 2, h - 2, TFT_DARKGREY); // inner border
}