#include "ui/pages/Clock/ClockPage.h"
#include "Config.h"
#include "ui/TopBar.h"
#include "ui/BottomBar.h"


void ClockPage::drawStaticLayout() {

    _tft.fillScreen(COLOR_BG);
    BottomBar bottomBar(_tft, 0);
    bottomBar.draw();

    _tft.setTextColor(COLOR_TEXT);
    _tft.setFreeFont(&FreeSansBold12pt7b);
    _tft.setTextDatum(MC_DATUM);  // Middle Center
    _tft.drawString("Monday, August 10", 240, 20);

    _tft.setTextColor(COLOR_TEXT);
    _tft.setFreeFont(&FreeSansBold24pt7b);
    _tft.drawString("20:24:14", 170, 150);

}
