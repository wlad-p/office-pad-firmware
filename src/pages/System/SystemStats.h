#pragma once
#include <cstdint> 

struct SystemStats{
    int     cpuPercent      = 0;
    int64_t ramTotalBytes   = 0;
    int64_t ramUsedBytes    = 0;
    int64_t swapTotalBytes  = 0;
    int64_t swapUsedBytes   = 0;
    int64_t diskTotalBytes  = 0;
    int64_t diskUsedBytes   = 0;
    bool    valid           = false;  // true once at least one packet received
};
