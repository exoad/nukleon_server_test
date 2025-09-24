#ifndef NUKLEON_PUBLIC_H
#define NUKLEON_PUBLIC_H

#include "nukleon_elements.h"
#include <stdint.h>
// --  Hollistic Game Rules

#define NK_RULE_STATE_FPS_TARGET 3
#define NK_RULE_TOTAL_TICKS_FOR_MELTDOWN_WIPE 5

// --- NkTicker

#define NK_STATE_RUN_SECONDS -1.0

NkVoid nkInitTime();

NkFloat64 nkTimeNowSec();

NkVoid nkSleepMs(NkInt32 ms);

NkVoid nkYield();

NkVoid nkGameLoop();

NkVoid nkUpdate(NkFloat64 dt);

typedef struct
{
    NkUInt16 row;
    NkUInt16 col;
} NkLocation;

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
    NkFloat64 totalPower;
    NkFloat64 maxPower;
    NkFloat64 totalHeat;
    NkFloat64 maxHeat;
    NkUInt16 rows;
    NkUInt16 cols;
    NkUInt16 capacityRows;
    NkUInt16 capacityCols;
    NkUInt64 tickIndex;
    NkUInt64 credits;
} NkGame;

extern NkGame gNkGameInstance;

NkUInt16 nkReactorGetWidth();

/// @brief For meltdown purposes
NkVoid nkResetReactor();

NkUInt16 nkReactorGetHeight();

NkVoid nkInitNkReactor(NkUInt16 width, NkUInt16 height);

/// @brief Get the number of all (including diagonal) neighbors of a cell
NkUInt16 nkReactorGetFullNeighborsOf(NkLocation loc);

/// @brief Get the number of orthogonal (up, down, left, right) neighbors of a cell
NkUInt16 nkReactorGetOrthoNeighborsOf(NkLocation loc);

/// @brief Free all allocated memory for the reactor grid
NkVoid nkUninitNkReactor();

/// @brief Request a pointer to the tile at the given row and column
NkTile* nkReactorGet(NkLocation loc);

/// @brief Copies the data of the given tile into the reactor grid at the given row and column
NkVoid nkReactorSet(NkLocation loc, NkTile other);

#endif