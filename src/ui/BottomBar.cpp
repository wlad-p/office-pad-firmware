#include "BottomBar.h"
#include "ui/icons/HomeIcon.h"

void BottomBar::draw()
{

    _tft.fillRect(0, Layout::screenHeight - Layout::bottomBarHeight, Layout::screenWidth, Layout::bottomBarHeight, COLOR_BG_DARK);
    // _tft.drawFastHLine(0, Layout::screenHeight - Layout::bottomBarHeight, Layout::screenWidth, TFT_WHITE);

    _tft.setFreeFont(&FreeSans9pt7b);
    _tft.setTextDatum(MC_DATUM); // Middle Center
    drawClock();
    drawMediaPlayer();
    drawCalendar();
    drawStats();
}

void BottomBar::drawClock()
{
    if (0 == index_)
    {
        _tft.drawLine(40, Layout::screenHeight - Layout::bottomBarHeight, 80, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
    }
    else
        _tft.setTextColor(COLOR_TEXT_GRAY);

    _tft.drawString("Home", 60, 310);
    _tft.pushImage(40, 270, 24, 24, (uint16_t*)icon_home);
}

void BottomBar::drawMediaPlayer()
{
    if (1 == index_)
    {

        _tft.drawLine(160, Layout::screenHeight - Layout::bottomBarHeight, 200, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
    }
    else
        _tft.setTextColor(COLOR_TEXT_GRAY);
    _tft.drawString("Media", 180, 310);
}

void BottomBar::drawCalendar()
{
    if (2 == index_)
    {

        _tft.drawLine(280, Layout::screenHeight - Layout::bottomBarHeight, 320, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
    }
    else
        _tft.setTextColor(COLOR_TEXT_GRAY);
    _tft.drawString("Calendar", 300, 310);
}

void BottomBar::drawStats()
{
    if (3 == index_)
    {
        _tft.drawLine(400, Layout::screenHeight - Layout::bottomBarHeight, 440, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
    }
    else
        _tft.setTextColor(COLOR_TEXT_GRAY);

    _tft.drawString("System", 420, 310);
}
