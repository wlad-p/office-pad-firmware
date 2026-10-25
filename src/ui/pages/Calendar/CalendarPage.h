#pragma once

#include <TFT_eSPI.h>
#include "ui/pages/IPage.h"
#include "ui/pages/Calendar/CalendarEvent.h"

class CalendarPage : public IPage
{
public:
    static const size_t MAX_EVENTS = 8;

    CalendarPage(uint8_t id, TFT_eSPI &tft) : IPage(id, tft) {}

    void drawStatic() override { drawStaticLayout(); }
    void update() override;   // top bar clock; redraws grid + agenda when the day changes

    void updateEvents(const CalendarEvent *events, size_t count);

private:
    void drawStaticLayout();
    void drawMonth(int32_t today);
    void drawAgenda(int32_t today);

    bool hasEventOn(int32_t day) const;
    int16_t drawEvent(const CalendarEvent &e, uint8_t index, int16_t y);

    CalendarEvent events_[MAX_EVENTS];
    size_t eventCount_ = 0;

    int16_t lastMinute_ = -1;
    int32_t shownDay_ = -1;   // day number the grid/agenda were drawn for, -1 = redraw
};
