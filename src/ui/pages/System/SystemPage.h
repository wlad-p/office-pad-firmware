#pragma once

#include <TFT_eSPI.h>
#include "Config.h"
#include "ui/pages/IPage.h"
#include "ui/pages/System/SystemStats.h"
#include "ui/pages/System/LayoutConfig.h"

class SystemPage : public IPage {
public:

    SystemPage(uint8_t id, TFT_eSPI& tft) : IPage(id, tft) {};
    void drawStatic() override {drawStaticLayout();};
    void update() override;   // keeps the clock in the top bar current

    void updateStats(SystemStats stats);

private:

    // What one card currently shows, so only changes get redrawn
    struct Tile {
        int x;
        int y;
        const char *title;
        int percent = -1;     // gauge value on screen, -1 = not drawn yet
        bool warn = false;    // label currently drawn in the warning colour
        char main[16] = "";
        char sub[16] = "";
    };

    Tile cpu_  {SystemUI::CPU_X, SystemUI::CPU_Y, "CPU"};
    Tile ram_  {SystemUI::RAM_X, SystemUI::RAM_Y, "RAM"};
    Tile swap_ {SystemUI::SWAP_X, SystemUI::SWAP_Y, "SWAP"};
    Tile disk_ {SystemUI::DISK_X, SystemUI::DISK_Y, "STORAGE"};
    int16_t lastMinute_ = -1;

    void drawStaticLayout();
    void drawStaticTile(Tile &tile);

    void updateCpu(float percent);
    void updateRam(float current, float total);
    void updateSwap(float current, float total);
    void updateDisk(float current, float total);

    void updateTile(Tile &tile, float percent, const char *main, const char *sub);
    void drawGauge(const Tile &tile, int percent, bool warn);
    void drawLabel(const Tile &tile, bool warn);
    void drawTextLine(const Tile &tile, const char *text, int y, const uint8_t *font, uint16_t color);
};
