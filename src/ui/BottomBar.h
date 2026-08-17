#pragma once
#include <cstdint>
#include <TFT_eSPI.h>
#include "Config.h"

class BottomBar{

    public:
    BottomBar(TFT_eSPI& tft, uint8_t index) : _tft(tft), index_(index){};
    BottomBar()=default;
    
    void draw();

    private:
    TFT_eSPI& _tft;
    uint8_t index_;

    void drawClock();
    void drawCalendar();
    void drawMediaPlayer();
    void drawStats();

};
