#pragma once

#include "pages/Calendar/CalendarPage.h"
#include "pages/Clock/ClockPage.h"
#include "pages/IPage.h"
#include "pages/Media/MediaPage.h"
#include "pages/System/SystemPage.h"
#include "pages/Startup/StartupPage.h"
#include <TFT_eSPI.h>
#include "pages/PageID.h"

class UIManager
{
public:
  UIManager(TFT_eSPI &tft)
      : _tft(tft), _current(nullptr), _clockPage(0, tft), _mediaPage(1, tft),
        _calendarPage(2, tft), _systemPage(3, tft), _startupPage(5, tft) {};

  UIManager() = default;

  StartupPage &startupPage() { return _startupPage; }
  MediaPage &mediaPage() { return _mediaPage; }
  SystemPage &systemPage() { return _systemPage; }
  ClockPage &clockPage() { return _clockPage; }
  CalendarPage &calendarPage() { return _calendarPage; }

  void init();
  void resetDisplay();
  void switchTo(PageID id);

  PageID currentPageID();
  TFT_eSPI &tft() { return _tft; };

private:
  TFT_eSPI &_tft;
  IPage *_current;

  StartupPage _startupPage;
  ClockPage _clockPage;
  MediaPage _mediaPage;
  CalendarPage _calendarPage;
  SystemPage _systemPage;

  IPage *pageForID(PageID id);
};
