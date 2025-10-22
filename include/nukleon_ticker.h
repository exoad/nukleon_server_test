#ifndef NUKLEON_TICKER_H
#define NUKLEON_TICKER_H

#include "nukleon_types.h"

Void nkInitTime();

Float64 nkTimeNowSec();

Void nkSleepMs(Int32 ms);

Void nkYield();

Void nkGameLoop();

UInt64 nkGetCurrentTickIndex();

Void nkUpdate(Float64 dt);

typedef struct {
    // -- tick life cycle information
    UInt64 tickIndex;
    UInt32 tickDurationMs;
    // -- actual game stats
    Float32 producedHeat;
    Float32 producedPower;
    Int32 meltdownTicker;
} NkGameTick;

NkGameTick* nkGetLastGameTick();

#endif