#include "SystemPage.h"
#include "pages/System/LayoutConfig.h"
#include "ui/BottomBar.h"
#include "ui/TopBar.h"
#include <ui/icons/CPUIcon.h>

void SystemPage::drawStaticLayout()
{
    _tft.fillScreen(COLOR_BG);
    BottomBar bottomBar(_tft, 3);
    bottomBar.draw();

    TopBar topBar(_tft);
    topBar.draw();

    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM); // Middle Center
    _tft.drawString("System Monitor", 15, Layout::topBarHeight + 8);

    drawStaticTile({.x = UI::CPU_X, .y = UI::CPU_Y, .title = "CPU"});
    drawStaticTile({.x = UI::RAM_X, .y = UI::RAM_Y, .title = "RAM"});
    drawStaticTile({.x = UI::SWAP_X, .y = UI::SWAP_Y, .title = "SWAP"});
    drawStaticTile({.x = UI::DISK_X, .y = UI::DISK_Y, .title = "DISK"});
}

void SystemPage::drawStaticTile(TileParams params)
{

    _tft.drawRoundRect(params.x, params.y, UI::TILE_WIDTH, UI::TILE_HEIGHT, 8, COLOR_BG_LIGHT);
    _tft.fillRoundRect(params.x, params.y, UI::TILE_WIDTH, UI::TILE_HEIGHT, 8, COLOR_BG_LIGHT);
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM);
    _tft.drawString(params.title, params.x + 10 + 18 + 8, params.y + 10);

    TFT_eSprite spr(&_tft);
    spr.createSprite(CPU_WIDTH, CPU_HEIGHT);
    spr.pushImage(0, 0, CPU_WIDTH, CPU_WIDTH, (const uint16_t *)cpu);
    spr.pushSprite(params.x + 10, params.y + 10); // display

    _tft.fillRoundRect(params.x + 10, params.y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, COLOR_TEXT_GRAY);
}

void SystemPage::updateStats(SystemStats stats)
{

    updateCpu(stats.cpuPercent);
    updateRam(stats.ramUsedGB, stats.ramTotalGB);
    updateSwap(stats.swapUsedGB, stats.swapTotalGB);
    updateDisk(stats.diskUsedGB, stats.diskTotalGB);
}

void SystemPage::updateCpu(float percent) {

    // Text
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(percent, 0, 2, used);

    _tft.fillRect(UI::CPU_X + 36, UI::CPU_Y + 30, 90, 50, COLOR_BG_LIGHT);

    _tft.drawString(used, UI::CPU_X + 36, UI::CPU_Y + 30);
    _tft.drawString("%", UI::CPU_X + 36 + 100, UI::CPU_Y + 30);

    if (percent > 100.0)
        percent = 100.0;
    else if(percent <= 0.0)
        percent = 0.1;
    int currentWidth = UI::BAR_WIDTH * percent / 100.0;

    _tft.fillRoundRect(UI::CPU_X + 10, UI::CPU_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, COLOR_TEXT_GRAY);
    _tft.fillRoundRect(UI::CPU_X + 10, UI::CPU_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, COLOR_PRIMARY);
};

void SystemPage::updateRam(float current, float total)
{

    // Text
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(current, 0, 2, used);

    _tft.fillRect(UI::RAM_X + 36, UI::RAM_Y + 30, 90, 50, COLOR_BG_LIGHT);

    _tft.drawString(used, UI::RAM_X + 36, UI::RAM_Y + 30);
    _tft.drawString("/GB", UI::RAM_X + 36 + 100, UI::RAM_Y + 30);

    // Progress Bar
    if (total <= 0)
        total = 1; // Prevent division by zero

    float percent = (float)current / (float)total;
    if (percent > 1.0)
        percent = 1.0;
    int currentWidth = UI::BAR_WIDTH * percent;

    _tft.fillRoundRect(UI::RAM_X + 10, UI::RAM_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, COLOR_TEXT_GRAY);
    _tft.fillRoundRect(UI::RAM_X + 10, UI::RAM_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, COLOR_PRIMARY);
};
void SystemPage::updateSwap(float current, float total)
{

    // Text
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(current, 0, 2, used);

    _tft.fillRect(UI::SWAP_X + 36, UI::SWAP_Y + 30, 90, 50, COLOR_BG_LIGHT);

    _tft.drawString(used, UI::SWAP_X + 36, UI::SWAP_Y + 30);
    _tft.drawString("/GB", UI::SWAP_X + 36 + 100, UI::SWAP_Y + 30);

    // Progress Bar
    if (total <= 0)
        total = 1; // Prevent division by zero

    float percent = (float)current / (float)total;
    if (percent > 1.0)
        percent = 1.0;
    int currentWidth = UI::BAR_WIDTH * percent;

    _tft.fillRoundRect(UI::SWAP_X + 10, UI::SWAP_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, COLOR_TEXT_GRAY);
    _tft.fillRoundRect(UI::SWAP_X + 10, UI::SWAP_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, COLOR_PRIMARY);
};
void SystemPage::updateDisk(float current, float total) {
// Text
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(current, 0, 2, used);

    _tft.fillRect(UI::DISK_X + 36, UI::DISK_Y + 30, 90, 50, COLOR_BG_LIGHT);

    _tft.drawString(used, UI::DISK_X + 36, UI::DISK_Y + 30);
    _tft.drawString("/GB", UI::DISK_X + 36 + 100, UI::DISK_Y + 30);

    // Progress Bar
    if (total <= 0)
        total = 1; // Prevent division by zero

    float percent = (float)current / (float)total;
    if (percent > 1.0)
        percent = 1.0;
    int currentWidth = UI::BAR_WIDTH * percent;

    _tft.fillRoundRect(UI::DISK_X + 10, UI::DISK_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, COLOR_TEXT_GRAY);
    _tft.fillRoundRect(UI::DISK_X + 10, UI::DISK_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, COLOR_PRIMARY);
};