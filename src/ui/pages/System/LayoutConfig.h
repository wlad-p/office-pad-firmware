// System page layout, 480x320 landscape (Sand design): 2x2 cards with a ring gauge each.
#pragma once

namespace SystemUI
{
    static const int TILE_WIDTH = 210;
    static const int TILE_HEIGHT = 118;
    static const int TILE_RADIUS = 14;

    static const int CPU_X = 24;
    static const int CPU_Y = 50;
    static const int RAM_X = CPU_X + TILE_WIDTH + 12;
    static const int RAM_Y = CPU_Y;
    static const int SWAP_X = CPU_X;
    static const int SWAP_Y = CPU_Y + TILE_HEIGHT + 12;
    static const int DISK_X = RAM_X;
    static const int DISK_Y = SWAP_Y;

    // Ring gauge, relative to the tile
    static const int GAUGE_CX = 56;
    static const int GAUGE_CY = TILE_HEIGHT / 2;
    static const int GAUGE_R = 41;          // outer radius
    static const int GAUGE_W = 8;           // ring thickness (inner radius must fit the number)

    // Text column, relative to the tile
    static const int TEXT_X = 112;
    static const int LABEL_Y = 36;          // vertical centres
    static const int MAIN_Y = 64;
    static const int SUB_Y = 90;
    static const int TEXT_W = TILE_WIDTH - TEXT_X - 8;

    static const int WARN_PERCENT = 80;     // gauge and label turn red from here
};
