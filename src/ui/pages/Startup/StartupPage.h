#pragma once

#include <TFT_eSPI.h>
#include "Config.h"
#include "ui/pages/IPage.h"

class StartupPage : public IPage {
public:

    StartupPage(uint8_t id, TFT_eSPI& tft) : IPage(id, tft) {};
    void drawStatic() override {drawStaticLayout();};


private:
    void drawStaticLayout();
  
};
