#pragma once
#include <stdint.h>

// One calendar event. Strings are copied in, so the event stays valid after the JSON is gone.
struct CalendarEvent
{
    char title[48] = "";
    char location[32] = "";

    uint16_t startYear = 0;
    uint8_t startMonth = 0, startDay = 0, startHour = 0, startMin = 0;
    uint16_t endYear = 0;
    uint8_t endMonth = 0, endDay = 0, endHour = 0, endMin = 0;

    bool allDay = false;
};
