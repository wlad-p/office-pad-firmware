#include "Config.h"


struct UI
{
    static const int TILE_WIDTH = 225;
    static const int TILE_HEIGHT = 100;

    static const int BAR_HEIGHT = 6;
    static const int BAR_WIDTH = 205;

    static const int CPU_X = 10;
    static const int CPU_Y = Layout::topBarHeight + 8 + 20;

    static const int RAM_X = 245;
    static const int RAM_Y =  Layout::topBarHeight + 8 + 20 ;

    static const int SWAP_X = 10;
    static const int SWAP_Y = Layout::topBarHeight + 8 + 20 + UI::TILE_HEIGHT + 8;

    static const int DISK_X = 245 ;
    static const int DISK_Y = Layout::topBarHeight + 8 + 20 + UI::TILE_HEIGHT + 8;

    
};
