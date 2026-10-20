#pragma once

#include <Arduino.h>

static constexpr uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

namespace Theme {
  constexpr uint16_t Background = rgb565(222, 196, 150t );  // warm cream
  constexpr uint16_t Panel      = rgb565(226, 214, 194);  // cards, pills, empty bars
  constexpr uint16_t Text       = rgb565(58, 44, 36);     // dark brown
  constexpr uint16_t SubText    = rgb565(130, 110, 92);   // secondary text
  constexpr uint16_t Accent     = rgb565(206, 92, 56);    // terracotta
  constexpr uint16_t Dim        = rgb565(200, 186, 164);  // inactive dots, outlines
  constexpr uint16_t LinkOk     = rgb565(60, 200, 110);   // green link dot
}

enum Page : uint8_t { PAGE_CLOCK, PAGE_CALENDAR, PAGE_MEDIA, PAGE_SYSTEM, PAGE_COUNT };




#define COLOR_PROGRESS_BG 0x3186
