#include "SystemPage.h"
#include "pages/System/LayoutConfig.h"
#include "ui/BottomBar.h"
#include "ui/TopBar.h"

void SystemPage::drawStaticLayout()
{
    _tft.fillScreen(COLOR_BG);
    BottomBar bottomBar(_tft, 3);
    bottomBar.draw();

    TopBar topBar(_tft);
    topBar.draw();

    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM); // Middle Center
    _tft.drawString("System Monitor", 15, Layout::topBarHeight + 10);
    _tft.drawString("LIVE", 400, Layout::topBarHeight + 10);

    drawStaticTile({.x=15, .y= 50, .title="CPU"});
    drawStaticTile({.x=230, .y= 50, .title="RAM"});
    drawStaticTile({.x=15, .y= 165, .title="SWAP"});
    drawStaticTile({.x=230, .y= 165, .title="STORAGE"});


}

void SystemPage::drawStaticTile(TileParams params){

    _tft.drawRect(params.x, params.y, 200, 100, TFT_RED);
    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM); 
    _tft.drawString(params.title, params.x + 10, params.y + 10);
}

void SystemPage::updateStats(SystemStats stats){
}
