#include "SystemPage.h"
#include "ui/pages/System/LayoutConfig.h"
#include "ui/TopBar.h"
#include <ui/icons/CPUIcon.h>

void SystemPage::drawStaticLayout()
{
    tft_.fillScreen(Theme::Background);

    TopBar topBar(tft_);
    topBar.draw(PAGE_SYSTEM, true, "TODO");

    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM); // Middle Center
    tft_.drawString("System Monitor", 15, Layout::topBarHeight + 8);

    drawStaticTile({.x = UI::CPU_X, .y = UI::CPU_Y, .title = "CPU"});
    drawStaticTile({.x = UI::RAM_X, .y = UI::RAM_Y, .title = "RAM"});
    drawStaticTile({.x = UI::SWAP_X, .y = UI::SWAP_Y, .title = "SWAP"});
    drawStaticTile({.x = UI::DISK_X, .y = UI::DISK_Y, .title = "DISK"});
}

void SystemPage::drawStaticTile(TileParams params)
{

    tft_.drawRoundRect(params.x, params.y, UI::TILE_WIDTH, UI::TILE_HEIGHT, 8, Theme::Background);
    tft_.fillRoundRect(params.x, params.y, UI::TILE_WIDTH, UI::TILE_HEIGHT, 8, Theme::Background);
    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM);
    tft_.drawString(params.title, params.x + 10 + 18 + 8, params.y + 10);

    TFT_eSprite spr(&tft_);
    spr.createSprite(CPU_WIDTH, CPU_HEIGHT);
    spr.pushImage(0, 0, CPU_WIDTH, CPU_WIDTH, (const uint16_t *)cpu);
    spr.pushSprite(params.x + 10, params.y + 10); // display

    tft_.fillRoundRect(params.x + 10, params.y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, Theme::Text);
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
    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(percent, 0, 2, used);

    tft_.fillRect(UI::CPU_X + 36, UI::CPU_Y + 30, 90, 50, Theme::Background);

    tft_.drawString(used, UI::CPU_X + 36, UI::CPU_Y + 30);
    tft_.drawString("%", UI::CPU_X + 36 + 100, UI::CPU_Y + 30);

    if (percent > 100.0)
        percent = 100.0;
    else if(percent <= 0.0)
        percent = 0.1;
    int currentWidth = UI::BAR_WIDTH * percent / 100.0;

    tft_.fillRoundRect(UI::CPU_X + 10, UI::CPU_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, Theme::Text);
    tft_.fillRoundRect(UI::CPU_X + 10, UI::CPU_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, Theme::Accent);
};

void SystemPage::updateRam(float current, float total)
{

    // Text
    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(current, 0, 2, used);

    tft_.fillRect(UI::RAM_X + 36, UI::RAM_Y + 30, 90, 50, Theme::Background);

    tft_.drawString(used, UI::RAM_X + 36, UI::RAM_Y + 30);
    tft_.drawString("/GB", UI::RAM_X + 36 + 100, UI::RAM_Y + 30);

    // Progress Bar
    if (total <= 0)
        total = 1; // Prevent division by zero

    float percent = (float)current / (float)total;
    if (percent > 1.0)
        percent = 1.0;
    int currentWidth = UI::BAR_WIDTH * percent;

    tft_.fillRoundRect(UI::RAM_X + 10, UI::RAM_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, Theme::Text);
    tft_.fillRoundRect(UI::RAM_X + 10, UI::RAM_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, Theme::Accent);
};
void SystemPage::updateSwap(float current, float total)
{

    // Text
    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(current, 0, 2, used);

    tft_.fillRect(UI::SWAP_X + 36, UI::SWAP_Y + 30, 90, 50, Theme::Background);

    tft_.drawString(used, UI::SWAP_X + 36, UI::SWAP_Y + 30);
    tft_.drawString("/GB", UI::SWAP_X + 36 + 100, UI::SWAP_Y + 30);

    // Progress Bar
    if (total <= 0)
        total = 1; // Prevent division by zero

    float percent = (float)current / (float)total;
    if (percent > 1.0)
        percent = 1.0;
    int currentWidth = UI::BAR_WIDTH * percent;

    tft_.fillRoundRect(UI::SWAP_X + 10, UI::SWAP_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, Theme::Text);
    tft_.fillRoundRect(UI::SWAP_X + 10, UI::SWAP_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, Theme::Accent);
};
void SystemPage::updateDisk(float current, float total) {
// Text
    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM);
    char used[16];
    dtostrf(current, 0, 2, used);

    tft_.fillRect(UI::DISK_X + 36, UI::DISK_Y + 30, 90, 50, Theme::Background);

    tft_.drawString(used, UI::DISK_X + 36, UI::DISK_Y + 30);
    tft_.drawString("/GB", UI::DISK_X + 36 + 100, UI::DISK_Y + 30);

    // Progress Bar
    if (total <= 0)
        total = 1; // Prevent division by zero

    float percent = (float)current / (float)total;
    if (percent > 1.0)
        percent = 1.0;
    int currentWidth = UI::BAR_WIDTH * percent;

    tft_.fillRoundRect(UI::DISK_X + 10, UI::DISK_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, Theme::Text);
    tft_.fillRoundRect(UI::DISK_X + 10, UI::DISK_Y + UI::TILE_HEIGHT - 16, currentWidth, UI::BAR_HEIGHT, 2, Theme::Accent);
};