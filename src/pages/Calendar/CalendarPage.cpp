#include "pages/Calendar/CalendarPage.h"
#include "Config.h"

void CalendarPage::drawStaticLayout() {
    _tft.fillScreen(TFT_RED);

    _tft.fillRect(0, 0, Layout::screenWidth, Layout::topBarHeight, TFT_BLACK);
    _tft.drawFastHLine(0, Layout::topBarHeight, Layout::screenWidth, TFT_BLACK);
    
    _tft.setTextColor(TFT_BLACK);
    _tft.drawString("NOW PLAYING", 15, 10, 2);

}
