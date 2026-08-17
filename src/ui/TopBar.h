#pragma once
#include <TFT_eSPI.h>
#include "Config.h"

class TopBar{
    public:
    TopBar(TFT_eSPI& tft) : _tft(tft) {};
    void draw();

    private:
    TFT_eSPI _tft;

};

