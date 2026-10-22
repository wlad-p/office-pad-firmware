// StartupPage.cpp
#include "StartupPage.h"
#include "fonts/FontLogo.h"
#include "fonts/FontDate.h"
#include "fonts/FontSmall.h"
#include "fonts/FontAlbum.h"

// ---------------------------------------------------------------------------
// Layout, 480x320 landscape
// ---------------------------------------------------------------------------
static const int16_t SCR_W = 480;
static const int16_t CX = SCR_W / 2;

static const int16_t STATUS_Y = 20;               // vertical centre of "Not connected"
static const int16_t LOGO_BASELINE = 132;
static const int16_t LINE_Y = 150, LINE_W = 56, LINE_H = 5;

static const int16_t CARD_X = 24, CARD_Y = 196, CARD_W = 432, CARD_H = 100;
static const int16_t ICON_CX = CARD_X + 44, ICON_CY = CARD_Y + CARD_H / 2;
static const int16_t TEXT_X = CARD_X + 86;        // left edge of the notice text

// ---------------------------------------------------------------------------

void StartupPage::drawStaticLayout()
{
    tft_.fillScreen(Theme::Background);
    drawStatus();
    drawLogo();
    drawNotice();
}

void StartupPage::drawStatus()
{
    // Terracotta dot + "Not connected" where the other pages show their page dots
    tft_.loadFont(FontSmall);
    tft_.setTextDatum(MR_DATUM);
    tft_.setTextColor(Theme::SubText, Theme::Background);
    const char *label = "Not connected";
    tft_.drawString(label, SCR_W - 20, STATUS_Y);
    int16_t dotX = SCR_W - 20 - tft_.textWidth(label) - 10;
    tft_.unloadFont();
    tft_.fillSmoothCircle(dotX, STATUS_Y, 4, Theme::Accent, Theme::Background);
}

void StartupPage::drawLogo()
{
    tft_.loadFont(FontLogo);
    tft_.setTextDatum(C_BASELINE);
    tft_.setTextColor(Theme::Text, Theme::Background);
    tft_.drawString("OfficePad", CX, LOGO_BASELINE);
    tft_.unloadFont();

    tft_.fillSmoothRoundRect(CX - LINE_W / 2, LINE_Y, LINE_W, LINE_H, 2, Theme::Accent, Theme::Background);
}

void StartupPage::drawNotice()
{
    const uint16_t bg = Theme::Panel;
    tft_.fillSmoothRoundRect(CARD_X, CARD_Y, CARD_W, CARD_H, 14, bg, Theme::Background);
    drawPlugIcon(ICON_CX, ICON_CY);

    tft_.setTextDatum(TL_DATUM);

    tft_.loadFont(FontDate);
    tft_.setTextColor(Theme::Text, bg);
    tft_.drawString("Software required", TEXT_X, CARD_Y + 18);
    tft_.unloadFont();

    tft_.loadFont(FontAlbum);
    tft_.setTextColor(Theme::SubText, bg);
    tft_.drawString("Download and start the OfficePad app", TEXT_X, CARD_Y + 48);
    tft_.drawString("on your computer to use this device.", TEXT_X, CARD_Y + 68);
    tft_.unloadFont();
}

void StartupPage::drawPlugIcon(int16_t cx, int16_t cy)
{
    tft_.fillSmoothCircle(cx, cy, 24, Theme::Background, Theme::Panel);
    const uint16_t c = Theme::Accent, bg = Theme::Background;
    tft_.fillRect(cx - 6, cy - 15, 3, 9, c);                          // prongs
    tft_.fillRect(cx + 4, cy - 15, 3, 9, c);
    tft_.fillSmoothRoundRect(cx - 10, cy - 7, 21, 15, 4, c, bg);      // body
    tft_.fillRect(cx - 2, cy + 7, 5, 9, c);                           // cable
}
