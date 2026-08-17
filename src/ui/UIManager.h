#pragma once

#include "pages/Calendar/CalendarPage.h"
#include "pages/Clock/ClockPage.h"
#include "pages/IPage.h"
#include "pages/Media/MediaPage.h"
#include "pages/System/SystemPage.h"
#include <TFT_eSPI.h>
#include "pages/PageID.h"



class UIManager
{
public:
  UIManager(TFT_eSPI &tft)
      : _tft(tft), _current(nullptr), clockPage_(0, tft), mediaPage_(1, tft),
        calendarPage_(2, tft), systemPage_(3, tft) {}

  MediaPage &mediaPage() { return mediaPage_; }
  SystemPage &systemPage() { return systemPage_; }
  ClockPage &clockPage() { return clockPage_; }
  CalendarPage &calendarPage() { return calendarPage_; }

  void init();
  void resetDisplay();
  void switchTo(PageID id);

  PageID currentPageID();

private:
  TFT_eSPI &_tft;
  IPage *_current;

  ClockPage clockPage_;
  MediaPage mediaPage_;
  CalendarPage calendarPage_;
  SystemPage systemPage_;

  IPage *pageForID(PageID id);
};
