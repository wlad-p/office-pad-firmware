#pragma once
#include <string.h>

enum NetworkCommand
{
    TRACK,
    PROGRESS,
    CALENDAR,
    MEDIA_ART,
    SYSTEM,
    CLOCK,
    UNKNOWN
};

inline NetworkCommand NetworkCommandFromString(const char* cmd)
{
    if (strcmp(cmd, "track") == 0)
        return TRACK;
    if (strcmp(cmd, "calendar") == 0)
        return CALENDAR;
    if (strcmp(cmd, "clock") == 0)
        return CLOCK;
    if (strcmp(cmd, "progress") == 0)
        return PROGRESS;
    if (strcmp(cmd, "media_art") == 0)
        return MEDIA_ART;
    if (strcmp(cmd, "system") == 0)
        return SYSTEM;
    return UNKNOWN;
}