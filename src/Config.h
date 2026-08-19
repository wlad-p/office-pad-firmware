// Config.h
#ifndef CONFIG_H
#define CONFIG_H

#include "ui/Colors.h"

// ------ INPUTS -------

// Buttons
const int BTN_PIN_MIC = 4;
const int BTN_PIN_MODE = 3;
const int BTN_PIN_SCREENSHOT = 21 ;
const int BTN_PIN_LOCK = 6;

const int BTN_PIN_F1 = 2;
const int BTN_PIN_F2 = 13;
const int BTN_PIN_F3 = 5;
const int BTN_PIN_F4 = 20;

const int BTN_PIN_PREV = 19;
const int BTN_PIN_PAUSE = 1;
const int BTN_PIN_NEXT = 0;

//ROTARY ENCODER
#define PIN_ENC_A   17
#define PIN_ENC_B   12
#define PIN_ENC_S1  17


// UI Layout Constants
struct Layout {
    static const int screenWidth  = 480;
    static const int screenHeight = 320;
    static const int topBarHeight = 32;
    static const int bottomBarHeight = 46;
};

#endif
