#pragma once

#include "pages/Calendar/CalendarPage.h"
#include "pages/Clock/ClockPage.h"
#include "pages/IPage.h"
#include "pages/Media/MediaPage.h"
#include "pages/System/SystemPage.h"
#include "pages/Startup/StartupPage.h"
#include <TFT_eSPI.h>
#include "pages/PageID.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include <Arduino.h>

class App
{
public:
  static App &instance()
  {
    static App inst;
    return inst;
  };

  App(const App &) = delete;
  App &operator=(const App &) = delete;

  void init();
  void update();

  // Clock
  void sync(int16_t year,
            int8_t month,
            int8_t day,
            int8_t dotw,
            int8_t hour,
            int8_t min,
            int8_t sec);

  datetime_t dateTime() const;


  // Pages
  PageID currentPageID();
  void switchTo(PageID id);
  TFT_eSPI &tft() { return tft_; };

  StartupPage &startupPage() { return startupPage_; }
  MediaPage &mediaPage() { return mediaPage_; }
  SystemPage &systemPage() { return systemPage_; }
  ClockPage &clockPage() { return clockPage_; }
  CalendarPage &calendarPage() { return calendarPage_; }

private:
  App() : tft_(),
          current_(nullptr),
          clockPage_(0, tft_),
          mediaPage_(1, tft_),
          calendarPage_(2, tft_),
          systemPage_(3, tft_),
          startupPage_(4, tft_) {};

  TFT_eSPI tft_;
  IPage *current_;

  StartupPage startupPage_;
  ClockPage clockPage_;
  MediaPage mediaPage_;
  CalendarPage calendarPage_;
  SystemPage systemPage_;

  IPage *pageForID(PageID id);
};
