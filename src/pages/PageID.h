#pragma once
#include <cstring>

enum PageID
{
  CLOCK_PAGE,
  MEDIA_PAGE,
  CALENDAR_PAGE,
  SYSTEM_PAGE
};


inline PageID pageIdFromString(const char *pageString)
{
  if (strcmp(pageString, "clock") == 0)
    return CLOCK_PAGE;
  if (strcmp(pageString, "media") == 0)
    return MEDIA_PAGE;
  if (strcmp(pageString, "calendar") == 0)
    return CALENDAR_PAGE;
  if (strcmp(pageString, "system") == 0)
    return SYSTEM_PAGE;

  return CLOCK_PAGE;
};

inline PageID pageIdFromInt(int id)
{
  switch (id)
  {
  case 0:
    return CLOCK_PAGE;
    break;
  case 1:
    return MEDIA_PAGE;
    break;
  case 2:
    return CALENDAR_PAGE;
    break;
  case 3:
    return SYSTEM_PAGE;
    break;
  default: break;
  }

  return CLOCK_PAGE;
};