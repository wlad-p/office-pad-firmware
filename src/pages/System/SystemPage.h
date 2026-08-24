#pragma once

#include <TFT_eSPI.h>
#include "Config.h"
#include "pages/IPage.h"
#include "pages/System/SystemStats.h"

class SystemPage : public IPage {
public:

    SystemPage(uint8_t id, TFT_eSPI& tft) : IPage(id, tft) {};
    void drawStatic(TFT_eSPI& tft) override {drawStaticLayout();};

    void updateStats(SystemStats stats);

private:

    struct TileParams{
        int x;
        int y;
        char* title;
    };

    void drawStaticTile(TileParams params);

    void drawStaticLayout();

    void updateCpu(float percent);
    void updateRam(float current, float total);
    void updateSwap(float current, float total);
    void updateDisk(float current, float total);
  
};
