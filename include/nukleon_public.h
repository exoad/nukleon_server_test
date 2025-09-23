#ifndef NUKLEON_PUBLIC_H
#define NUKLEON_PUBLIC_H

#include "nukleon_elements.h"
#include <stdint.h>

// --- NkTicker

#define NK_STATE_TPS_TARGET 6
#define NK_STATE_RUN_SECONDS -1.0

NkVoid nkInitTime();

NkFloat64 nkTimeNowSec();

NkVoid nkSleepMs(NkInt32 ms);

NkVoid nkYield();

NkVoid nkGameLoop();

NkVoid nkUpdate(NkFloat64 dt);

// --- NkTickStats

typedef struct
{
    NkUInt64 tickIndex;
    NkFloat32 producedPower;
    NkFloat32 producedHeat;
    NkInt32 activeTiles;
} NkTickStats;

// --- NkGame

#define NK_REACTOR_DEFAULT_START_HEAT 0.0
#define NK_REACTOR_DEFAULT_MAX_HEAT 1000.0

typedef struct
{
    NkTile** reactor;
    NkFloat32 totalPower;
    NkFloat32 maxPower;
    NkFloat32 totalHeat;
    NkFloat32 maxHeat;
    NkUInt16 rows;
    NkUInt16 cols;
    NkUInt16 capacityRows;
    NkUInt16 capacityCols;
    NkUInt64 tickIndex;
    NkUInt64 credits;
} NkGame;

extern NkGame nkGameInstance;

NkUInt16 nkReactorGetWidth();

NkUInt16 nkReactorGetHeight();

NkVoid nkInitNkReactor(NkUInt16 width, NkUInt16 height);

NkVoid nkUninitNkReactor();

NkTile* nkReactorGet(NkUInt16 row, NkUInt16 col);

NkVoid nkReactorSet(NkUInt16 row, NkUInt16 col, NkTile other);

#endif