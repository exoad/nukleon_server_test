#ifndef NUKLEON_TICKER_H
#define NUKLEON_TICKER_H

#include "nukleon_types.h"

NkVoid nkInitTime();

NkFloat64 nkTimeNowSec();

NkVoid nkSleepMs(NkInt32 ms);

NkVoid nkYield();

NkVoid nkGameLoop();

NkUInt64 nkGetCurrentTickIndex();

NkVoid nkUpdate(NkFloat64 dt);

typedef struct {
    // -- tick life cycle information
    NkUInt64 tickIndex;
    NkUInt32 tickDurationMs;
    // -- actual game stats
    NkFloat32 producedHeat;
    NkFloat32 producedPower;
    NkInt32 meltdownTicker;
} NkGameTick;

NkGameTick* nkGetLastGameTick();

#endif