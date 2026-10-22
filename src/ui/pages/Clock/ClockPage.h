#pragma once

#include <TFT_eSPI.h>
#include "Config.h"
#include "ui/pages/IPage.h"

class ClockPage : public IPage
{
public:
    ClockPage(uint8_t id, TFT_eSPI &tft) : IPage(id, tft) {};
    void drawStatic() override { drawStaticLayout(); };

    void update() override;

private:
    void drawStaticLayout();

    void drawThermometer();
    void drawClockTemperature(const char *value);

    void drawTime(uint8_t hour, uint8_t min);
    void drawSeconds(uint8_t sec);
    void drawDate(uint16_t year, uint8_t month, uint8_t day, uint8_t dotweek);
    const char *dotwToString(uint8_t dotw);

    int8_t lastMin_ = -1;
    int8_t lastDay_ = -1;
    int16_t lastBarWidth_ = -1; // must be signed
};