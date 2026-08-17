// MediaPage.cpp
#include "MediaPage.h"
#include "pages/Media/LayoutConfig.h"
#include "ui/TopBar.h"
#include "ui/BottomBar.h"

void MediaPage::drawStaticLayout()
{
    _tft.fillScreen(COLOR_BG);
    BottomBar bottomBar(_tft, 1);
    bottomBar.draw();

    TopBar topBar(_tft);
    topBar.draw();

    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM); // Middle Center
    _tft.drawString("NOW PLAYING", UI::artX + UI::artSize + 5, UI::artY);

    // Album Art Frame
    _tft.fillRoundRect(UI::artX, UI::artY, UI::artSize, UI::artSize, 8, TFT_RED);
    _tft.drawRoundRect(UI::artX, UI::artY, UI::artSize, UI::artSize, 8, COLOR_PROGRESS_BG);
    _tft.fillCircle(UI::artX + (UI::artSize / 2), UI::artY + (UI::artSize / 2), 55, COLOR_BG);
    _tft.fillCircle(UI::artX + (UI::artSize / 2), UI::artY + (UI::artSize / 2), 18, COLOR_PRIMARY);

    updateTrack("Ausgehen", "AnnenMayKanthereit","Schlagschatten");
    updateProgress(20,100);

}

void MediaPage::updateTrack(const char *title, const char *artist, const char *album)
{
    clearMetadataArea();

    // Title
    _tft.setTextColor(COLOR_TEXT);
    _tft.drawString(title, UI::contentX, UI::titleY, 4);

    // Artist
    _tft.setTextColor(COLOR_TEXT_GRAY);
    _tft.drawString(artist, UI::contentX, UI::artistY, 2);

    // Album
    if (album && strlen(album) > 0)
    {
        _tft.drawString(album, UI::contentX, UI::albumY + 20, 1);
    }
}

void MediaPage::clearMetadataArea()
{
    // Overwrite old text area with the background color before drawing new text
    int width = Layout::screenWidth - UI::contentX;
    _tft.fillRect(UI::contentX, UI::titleY, width, 100, COLOR_BG);
}


void MediaPage::updateProgress(int currentSec, int totalSec) {
    if (totalSec <= 0) totalSec = 1; // Prevent division by zero
    
    float pct = (float)currentSec / (float)totalSec;
    if (pct > 1.0) pct = 1.0;
    int currentWidth = UI::barWidth * pct;

    // Redraw Progress Bar
    _tft.fillRect(UI::barX, UI::barY, currentWidth, UI::barHeight, COLOR_PRIMARY);
    _tft.fillRect(UI::barX + currentWidth, UI::barY, UI::barWidth - currentWidth, UI::barHeight, COLOR_PROGRESS_BG);

    // Format timestamps
    char elapsed[8];
    char remaining[8];
    sprintf(elapsed, "%d:%02d", currentSec / 60, currentSec % 60);
    sprintf(remaining, "%d:%02d", totalSec / 60, totalSec % 60);

    _tft.setTextColor(COLOR_TEXT_GRAY, COLOR_BG);
    _tft.drawString(elapsed, UI::barX, UI::barY + 12, 2);
    
    int rightTextWidth = _tft.textWidth(remaining, 2);
    _tft.drawString(remaining, (UI::barX + UI::barWidth) - rightTextWidth, UI::barY + 12, 2);
}
