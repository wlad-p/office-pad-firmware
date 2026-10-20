#include "ui/TopBar.h"
#include "Config.h"

void TopBar::draw(Page active, bool linkOk, const char *time) {
  const uint16_t bg = Theme::Background;
 
  if (time) {
    tft_.loadFont(FontDate);
    tft_.setTextDatum(ML_DATUM);
    tft_.setTextColor(Theme::Text, bg);
    tft_.drawString(time, 24, Layout::topBarHeight);
    tft_.unloadFont();
  }
 
  // Link dot
  tft_.fillSmoothCircle(380, Layout::topBarHeight, 4, linkOk ? Theme::LinkOk : Theme::Dim, bg);
 
  // Page indicator: the active page is a pill, the others are dots
  int16_t x = 400;
  for (uint8_t i = 0; i < PAGE_COUNT; i++) {
    if (i == active) {
      tft_.fillSmoothRoundRect(x, Layout::topBarHeight - 3, 22, 7, 3, Theme::Accent, bg);
      x += 30;
    } else {
      tft_.fillSmoothCircle(x + 3, Layout::topBarHeight, 3, Theme::Dim, bg);
      x += 14;
    }
  }
}
 
