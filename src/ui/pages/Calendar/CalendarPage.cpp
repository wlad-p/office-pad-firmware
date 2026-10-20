#include "ui/pages/Calendar/CalendarPage.h"
#include "Config.h"
#include <ui/TopBar.h>
#include "Layout.h"

void CalendarPage::drawStaticLayout() {
    tft_.fillScreen(Theme::Background);

    TopBar topBar(tft_);
    topBar.draw(PAGE_CALENDAR, false, "");

    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM);
    tft_.drawString("Upcoming Events", 15, Layout::topBarHeight + 8);
}

void CalendarPage::updateEvents(const CalendarEvent events[], size_t count){
    
    for(int i = 0 ; i < count; i++){
        drawTile(10, 30 + 100 * i, events[count]);
    }
}

void CalendarPage::drawTile(int x, int y, CalendarEvent event){
    tft_.drawRoundRect(x, y, UI::TILE_WIDTH, UI::TILE_HEIGHT, 8, Theme::Background);
    tft_.setTextColor(TFT_WHITE);
    tft_.setTextDatum(TL_DATUM);
    tft_.drawString(event.title, x + 50, y + 10);
}
