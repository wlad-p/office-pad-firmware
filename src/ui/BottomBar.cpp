#include "BottomBar.h"
#include "ui/icons/HomeIcon.h"
#include "icons/CalendarIcon.h"
#include "icons/SystemIcon.h"
#include "icons/Mediacon.h"

void BottomBar::draw()
{

    _tft.fillRect(0, Layout::screenHeight - Layout::bottomBarHeight, Layout::screenWidth, Layout::bottomBarHeight, COLOR_BG_DARK);
    // _tft.drawFastHLine(0, Layout::screenHeight - Layout::bottomBarHeight, Layout::screenWidth, TFT_WHITE);

    _tft.setFreeFont(&FreeSans9pt7b);
    _tft.setTextDatum(MC_DATUM); // Middle Center
    drawClock();
    drawMediaPlayer();
    drawCalendar();
    drawSystem();
}

void BottomBar::drawClock()
{
    TFT_eSprite spr(&_tft);
    spr.createSprite(HOME_WIDTH, HOME_HEIGHT);
    if (0 == index_)
    {
        _tft.drawLine(40, Layout::screenHeight - Layout::bottomBarHeight, 80, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
        spr.pushImage(0, 0, HOME_WIDTH, HOME_HEIGHT, (const uint16_t *)home);
    }
    else
    {
        _tft.setTextColor(COLOR_TEXT_GRAY);
        spr.pushImage(0, 0, HOME_WIDTH, HOME_HEIGHT, (const uint16_t *)home);
    }

    spr.pushSprite(42, Layout::screenHeight - HOME_HEIGHT - 3); // display
}

void BottomBar::drawMediaPlayer()
{
    TFT_eSprite spr(&_tft);
    spr.createSprite(MEDIA_WIDTH, MEDIA_HEIGHT);
    if (1 == index_)
    {
        _tft.drawLine(160, Layout::screenHeight - Layout::bottomBarHeight, 200, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
        spr.pushImage(0, 0, MEDIA_WIDTH, MEDIA_HEIGHT, (const uint16_t *)media);
    }
    else
    {
        _tft.setTextColor(COLOR_TEXT_GRAY);
        spr.pushImage(0, 0, MEDIA_WIDTH, MEDIA_HEIGHT, (const uint16_t *)media);
    }

    spr.pushSprite(162, Layout::screenHeight - MEDIA_HEIGHT - 3); // display
}

void BottomBar::drawCalendar()
{
    TFT_eSprite spr(&_tft);
    spr.createSprite(CALENDAR_WIDTH, CALENDAR_HEIGHT);
    if (2 == index_)
    {
        _tft.drawLine(280, Layout::screenHeight - Layout::bottomBarHeight, 320, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
        spr.pushImage(0, 0, CALENDAR_WIDTH, CALENDAR_HEIGHT, (const uint16_t *)calendar);
    }
    else
    {
        _tft.setTextColor(COLOR_TEXT_GRAY);
        spr.pushImage(0, 0, CALENDAR_WIDTH, CALENDAR_HEIGHT, (const uint16_t *)calendar);
    }

    spr.pushSprite(282, Layout::screenHeight - CALENDAR_HEIGHT - 3); // display
}

void BottomBar::drawSystem()
{
    TFT_eSprite spr(&_tft);
    spr.createSprite(SYSTEM_WIDTH, SYSTEM_HEIGHT);
    if (3 == index_)
    {
        _tft.drawLine(400, Layout::screenHeight - Layout::bottomBarHeight, 440, Layout::screenHeight - Layout::bottomBarHeight, COLOR_PRIMARY);
        _tft.setTextColor(COLOR_PRIMARY);
        spr.pushImage(0, 0, SYSTEM_WIDTH, SYSTEM_HEIGHT, (const uint16_t *)system_icon);
    }
    else
    {
        _tft.setTextColor(COLOR_TEXT_GRAY);
        spr.pushImage(0, 0, SYSTEM_WIDTH, SYSTEM_HEIGHT, (const uint16_t *)system_icon);
    }

    spr.pushSprite(402, Layout::screenHeight - SYSTEM_HEIGHT - 3); // display
}
