#pragma once
#include <TFT_eSPI.h>
#include "Config.h"

class TopBar{
    public:
    TopBar(TFT_eSPI& tft) : tft_(tft) {};
    void draw(Page active, bool linkOk, const char *time);


    private:
    TFT_eSPI tft_;

};

