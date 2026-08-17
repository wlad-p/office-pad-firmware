#pragma once
#include <string.h>

enum NetworkCommand
{
    TRACK,
    PROGRESS,
    ART_SIZE,
    SYSTEM,
    UNKNOWN
};

inline NetworkCommand NetworkCommandFromString(const char* cmd)
{
    if (strcmp(cmd, "track") == 0)
        return TRACK;
    if (strcmp(cmd, "progess") == 0)
        return PROGRESS;
    if (strcmp(cmd, "art_size") == 0)
        return ART_SIZE;
    if (strcmp(cmd, "system") == 0)
        return SYSTEM;
    return UNKNOWN;
}