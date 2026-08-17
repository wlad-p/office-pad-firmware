#include "ui/TopBar.h"
#include "Config.h"

void TopBar::draw(){
    _tft.fillRect(0, 0, Layout::screenWidth, Layout::topBarHeight, COLOR_BG_DARK);
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM); // Middle Center
    _tft.setFreeFont(&FreeSans9pt7b);
    _tft.drawString("20:23", 15, 10);
};

