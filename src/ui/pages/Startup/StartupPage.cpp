#include "ui/pages/Startup/StartupPage.h"

void StartupPage::drawStaticLayout(){
    tft_.fillScreen(Theme::Background);

    tft_.setTextColor(Theme::Text);
    tft_.setTextDatum(TL_DATUM);
    tft_.drawString("No Connection Found. Install and configure the OfficePad Application on your PC.", 15, 200);

}