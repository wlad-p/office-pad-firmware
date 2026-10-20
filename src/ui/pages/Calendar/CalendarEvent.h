#pragma once
#include <cstdint>

#define TITLE_LENGTH    64
#define LOCATION_LENGTH 64

struct CalendarEvent {
    const char* title;
    const char* location;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    bool allDay;
};