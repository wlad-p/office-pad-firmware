#include "ui/pages/Calendar/CalendarPage.h"
#include "Config.h"
#include <ui/BottomBar.h>
#include <ui/TopBar.h>

void CalendarPage::drawStaticLayout() {
    _tft.fillScreen(COLOR_BG);
    BottomBar bottomBar(_tft, 2);
    bottomBar.draw();

    TopBar topBar(_tft);
    topBar.draw();

    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM);
    _tft.drawString("Upcoming Events", 15, Layout::topBarHeight + 8);
}
