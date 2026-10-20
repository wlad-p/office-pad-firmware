#pragma once

#include <TFT_eSPI.h>
#include "Config.h"
#include "ui/pages/IPage.h"
#include "ui/pages/Calendar/CalendarEvent.h"

class CalendarPage : public IPage {
public:

    CalendarPage(uint8_t id, TFT_eSPI& tft) : IPage(id, tft) {};
    void drawStatic() override {drawStaticLayout();};
    void updateEvents(const CalendarEvent events[], size_t count);

private:

    void drawStaticLayout();
    void drawTile(int x, int y, CalendarEvent event);
  
};
