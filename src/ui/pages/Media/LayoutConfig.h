// Media page layout, 480x320 landscape (Sand design).
#pragma once
#include <stdint.h>
#include "Config.h"

namespace UI {
  // Album art: 140x140 RGB565 = 39200 bytes, matches NetworkInterface::handleMediaArt
  constexpr int16_t artX = 24;
  constexpr int16_t artY = 58;
  constexpr int16_t artSize = 140;

  // Track info, right of the art
  constexpr int16_t contentX = artX + artSize + 22;                // 186
  constexpr int16_t contentW = Layout::screenWidth - 24 - contentX;
  constexpr int16_t labelY = artY + 6;                             // "NOW PLAYING" (top)
  constexpr int16_t titleY = artY + 30;                            // title (top)
  constexpr int16_t artistY = titleY + 34;
  constexpr int16_t albumY = artistY + 28;

  // Progress bar + times
  constexpr int16_t barX = 24;
  constexpr int16_t barY = 238;
  constexpr int16_t barWidth = Layout::screenWidth - 48;
  constexpr int16_t barHeight = 6;
  constexpr int16_t timeY = barY + 26;                             // vertical centre of the times
}
