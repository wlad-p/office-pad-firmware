#include "ui/pages/Startup/StartupPage.h"

void StartupPage::drawStaticLayout(){
    _tft.fillScreen(COLOR_BG);

    _tft.setTextColor(COLOR_TEXT);
    _tft.setTextDatum(TL_DATUM);
    _tft.drawString("No Connection Found. Install and configure the OfficePad Application on your PC.", 15, 200);

}