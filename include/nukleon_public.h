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
    NkInt32 rows;
    NkInt32 cols;
    NkInt32 capacityRows;
    NkInt32 capacityCols;
    NkUInt64 tickIndex;
    NkUInt64 credits;
} NkGame;

extern NkGame nkGameInstance;

NkInt16 nkReactorGetWidth();

NkInt16 nkReactorGetHeight();

NkVoid nkInitNkReactor(NkInt16 width, NkInt16 height);

NkVoid nkUninitNkReactor();

NkTile* nkReactorGet(NkInt16 row, NkInt16 col);

NkVoid nkReactorSet(NkInt16 row, NkInt16 col, NkTile other);

#endif