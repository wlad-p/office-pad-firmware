#include "SystemPage.h"
#include "ui/TopBar.h"
#include "ui/App.h"
#include "fonts/FontTemp.h"
#include "fonts/FontDate.h"
#include "fonts/FontSmall.h"
#include "fonts/FontAlbum.h"

// Gauge sweep: 270° with the gap at the bottom (TFT_eSPI angles: 0 = 6 o'clock, clockwise)
static const int ARC_START = 45;
static const int ARC_END = 315;

void SystemPage::drawStaticLayout()
{
    tft_.fillScreen(Theme::Background);
    lastMinute_ = -1;
    update();   // top bar with the current time

    drawStaticTile(cpu_);
    drawStaticTile(ram_);
    drawStaticTile(swap_);
    drawStaticTile(disk_);
}

void SystemPage::update()
{
    datetime_t dt = App::instance().dateTime();
    if (dt.min == lastMinute_) return;
    lastMinute_ = dt.min;

    char hm[6];
    snprintf(hm, sizeof(hm), "%02u:%02u", (unsigned)dt.hour, (unsigned)dt.min);
    TopBar topBar(tft_);
    topBar.draw(PAGE_SYSTEM, true, hm);
}

void SystemPage::drawStaticTile(Tile &tile)
{
    tft_.fillSmoothRoundRect(tile.x, tile.y, SystemUI::TILE_WIDTH, SystemUI::TILE_HEIGHT, SystemUI::TILE_RADIUS, Theme::Panel, Theme::Background);

    // Forget what was on screen so the next update draws everything
    tile.percent = -1;
    tile.warn = false;
    tile.main[0] = '\0';
    tile.sub[0] = '\0';

    drawGauge(tile, 0, false);
    drawLabel(tile, false);
    drawTextLine(tile, "--", SystemUI::MAIN_Y, FontDate, Theme::Text);
}

void SystemPage::updateStats(SystemStats stats)
{
    updateCpu(stats.cpuPercent);
    updateRam(stats.ramUsedGB, stats.ramTotalGB);
    updateSwap(stats.swapUsedGB, stats.swapTotalGB);
    updateDisk(stats.diskUsedGB, stats.diskTotalGB);
}

void SystemPage::updateCpu(float percent)
{
    char num[12], main[16];
    dtostrf(percent, 0, 1, num);   // no %f: printf float support is often left out of the Pico build
    snprintf(main, sizeof(main), "%s %%", num);
    updateTile(cpu_, percent, main, "");
}

// RAM, swap and disk share one layout: "10.9 GB" / "of 16 GB"
static void formatUsage(float current, float total, char *main, char *sub, size_t len)
{
    // dtostrf instead of %f: printf float support is often left out of the Pico build
    const int decimals = total >= 100 ? 0 : 1;   // "760 GB" but "10.9 GB"
    char cur[12], tot[12];
    dtostrf(current, 0, decimals, cur);
    dtostrf(total, 0, decimals, tot);
    snprintf(main, len, "%s GB", cur);
    snprintf(sub, len, "of %s GB", tot);
}

static float usagePercent(float current, float total)
{
    return total > 0 ? current * 100.0f / total : 0.0f;
}

void SystemPage::updateRam(float current, float total)
{
    char main[16], sub[16];
    formatUsage(current, total, main, sub, sizeof(main));
    updateTile(ram_, usagePercent(current, total), main, sub);
}

void SystemPage::updateSwap(float current, float total)
{
    char main[16], sub[16];
    formatUsage(current, total, main, sub, sizeof(main));
    updateTile(swap_, usagePercent(current, total), main, sub);
}

void SystemPage::updateDisk(float current, float total)
{
    char main[16], sub[16];
    formatUsage(current, total, main, sub, sizeof(main));
    updateTile(disk_, usagePercent(current, total), main, sub);
}

// ---------------------------------------------------------------------------
// Drawing, only what changed
// ---------------------------------------------------------------------------

void SystemPage::updateTile(Tile &tile, float percent, const char *main, const char *sub)
{
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    int p = (int)(percent + 0.5f);
    bool warn = p >= SystemUI::WARN_PERCENT;

    if (p != tile.percent || warn != tile.warn)
    {
        drawGauge(tile, p, warn);
        tile.percent = p;
    }
    if (warn != tile.warn)
    {
        drawLabel(tile, warn);
        tile.warn = warn;
    }
    if (strcmp(main, tile.main) != 0)
    {
        drawTextLine(tile, main, SystemUI::MAIN_Y, FontDate, Theme::Text);
        strncpy(tile.main, main, sizeof(tile.main) - 1);
    }
    if (strcmp(sub, tile.sub) != 0)
    {
        drawTextLine(tile, sub, SystemUI::SUB_Y, FontAlbum, Theme::SubText);
        strncpy(tile.sub, sub, sizeof(tile.sub) - 1);
    }
}

void SystemPage::drawGauge(const Tile &tile, int percent, bool warn)
{
    const int cx = tile.x + SystemUI::GAUGE_CX, cy = tile.y + SystemUI::GAUGE_CY;
    const int r = SystemUI::GAUGE_R, ir = SystemUI::GAUGE_R - SystemUI::GAUGE_W;
    const int split = ARC_START + (ARC_END - ARC_START) * percent / 100;

    // Filled part and empty part never overlap, so redrawing both doesn't flicker
    if (split > ARC_START)
        tft_.drawSmoothArc(cx, cy, r, ir, ARC_START, split, warn ? Theme::Warn : Theme::Accent, Theme::Panel, false);
    if (split < ARC_END)
        tft_.drawSmoothArc(cx, cy, r, ir, split, ARC_END, Theme::Dim, Theme::Panel, false);

    // Percentage in the middle
    char num[4];
    snprintf(num, sizeof(num), "%d", percent);
    tft_.loadFont(FontTemp);
    tft_.setTextDatum(MC_DATUM);
    tft_.setTextColor(Theme::Text, Theme::Panel, true);
    tft_.setTextPadding(tft_.textWidth("100"));
    tft_.drawString(num, cx, cy - 2);
    tft_.setTextPadding(0);
    tft_.unloadFont();

    tft_.loadFont(FontSmall);
    tft_.setTextDatum(MC_DATUM);
    tft_.setTextColor(Theme::SubText, Theme::Panel, true);
    tft_.drawString("%", cx, cy + 20);
    tft_.unloadFont();
}

void SystemPage::drawLabel(const Tile &tile, bool warn)
{
    drawTextLine(tile, tile.title, SystemUI::LABEL_Y, FontSmall, warn ? Theme::Warn : Theme::SubText);
}

void SystemPage::drawTextLine(const Tile &tile, const char *text, int y, const uint8_t *font, uint16_t color)
{
    tft_.loadFont(font);
    tft_.setTextDatum(ML_DATUM);
    tft_.setTextColor(color, Theme::Panel, true);
    tft_.setTextPadding(SystemUI::TEXT_W);   // clears the rest of the line, no fillRect needed
    tft_.drawString(text, tile.x + SystemUI::TEXT_X, tile.y + y);
    tft_.setTextPadding(0);
    tft_.unloadFont();
}
