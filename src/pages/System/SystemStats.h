#pragma once
#include <cstdint> 

struct SystemStats{
    float     cpuPercent      = 0.0f;
    float ramTotalGB   = 0.0f;
    float ramUsedGB    = 0.0f;
    float swapTotalGB  = 0.0f;
    float swapUsedGB   = 0.0f;
    float diskTotalGB  = 0.0f;
    float diskUsedGB   = 0.0f;
    bool    valid           = false;  // true once at least one packet received
};
