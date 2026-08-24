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

    drawStaticTile({.x=UI::CPU_X, .y=  UI::CPU_Y, .title="CPU"});
    drawStaticTile({.x=UI::RAM_X, .y=  UI::RAM_Y, .title="RAM"});
    drawStaticTile({.x=UI::SWAP_X, .y= UI::SWAP_Y , .title="SWAP"});
    drawStaticTile({.x=UI::STORAGE_X, .y=  UI::STORAGE_Y, .title="STORAGE"});

}

void SystemPage::drawStaticTile(TileParams params){

    _tft.drawRoundRect(params.x, params.y, UI::TILE_WIDTH, UI::TILE_HEIGHT, 8, COLOR_BG_LIGHT);
    _tft.fillRoundRect(params.x, params.y, UI::TILE_WIDTH, UI::TILE_HEIGHT, 8, COLOR_BG_LIGHT);
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM); 
    _tft.drawString(params.title, params.x + 10 + 18 + 8, params.y + 10);

    TFT_eSprite spr(&_tft);
    spr.createSprite(CPU_WIDTH, CPU_HEIGHT);
    spr.pushImage(0, 0, CPU_WIDTH, CPU_WIDTH, (const uint16_t *)cpu);
    spr.pushSprite(params.x + 10, params.y + 10); // display

    _tft.fillRoundRect(params.x+10, params.y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, COLOR_TEXT_GRAY);

    // _tft.fillRect(UI::barX, UI::barY, currentWidth, UI::barHeight, COLOR_PRIMARY);
    // _tft.fillRect(UI::barX + currentWidth, UI::barY, UI::barWidth - currentWidth, UI::barHeight, COLOR_PROGRESS_BG);


}

void SystemPage::updateStats(SystemStats stats){

    updateCpu(stats.cpuPercent);
    updateRam(stats.ramUsedBytes, stats.ramTotalBytes);

}

void SystemPage::updateCpu(int percent){

};

void SystemPage::updateRam(int current, int total){

    // Write text

    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM); 
    char used[8];
    sprintf(used, "%d", current);

    _tft.drawString(used, UI::RAM_X + 36, UI::RAM_Y + 30);
    _tft.drawString("/GB", UI::RAM_X + 36 + 100, UI::RAM_Y + 30);


    if (total <= 0) total = 1; // Prevent division by zero
    
    float percent = (float)current/ (float)total;
    if (percent > 1.0) percent = 1.0;
    int currentWidth = UI::BAR_WIDTH * percent;

    // Redraw Progress Bar
    _tft.fillRoundRect(UI::RAM_X+10, UI::RAM_Y + UI::TILE_HEIGHT - 16, UI::BAR_WIDTH, UI::BAR_HEIGHT, 2, COLOR_TEXT_GRAY);
    _tft.fillRoundRect(UI::RAM_X+10, UI::RAM_Y + UI::TILE_HEIGHT - 16, UI::RAM_X+10 + currentWidth, UI::BAR_HEIGHT, 2, COLOR_PRIMARY);



    // _tft.fillRect(UI::CPU_BAR_X, UI::CPU_BAR_Y, currentWidth, UI::BAR_HEIGHT, COLOR_ACCENT);
    // _tft.fillRect(UI::CPU_BAR_X + currentWidth, UI::CPU_BAR_Y, UI::BAR_WIDTH - currentWidth, UI::BAR_HEIGHT, COLOR_PROGRESS_BG);


};
void SystemPage::updateSwap(int current, int total){

};
void SystemPage::updateStorage(int current, int total){

};

