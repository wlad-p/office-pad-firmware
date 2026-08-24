#pragma once

#include <TFT_eSPI.h>
#include "Config.h"
#include "ui/pages/IPage.h"

class CalendarPage : public IPage {
public:

    CalendarPage(uint8_t id, TFT_eSPI& tft) : IPage(id, tft) {};
    void drawStatic(TFT_eSPI& tft) override {drawStaticLayout();};

private:

    void drawStaticLayout();
  
};
