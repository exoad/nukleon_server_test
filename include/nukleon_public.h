#ifndef NUKLEON_PUBLIC_H
#define NUKLEON_PUBLIC_H

#include "nukleon_elements.h"
#include "nukleon_sampler.h"
#include <stdint.h>
// --  Hollistic Game Rules

#define NK_RULE_STATE_FPS_TARGET 18
#define NK_RULE_TOTAL_TICKS_FOR_MELTDOWN_WIPE 5
#define NK_RULE_GAME_REACTOR_STARTING_MAX_HEAT 1000.0
#define NK_RULE_GAME_REACTOR_STARTING_MAX_POWER 100.0

// --- NkTicker

#define NK_STATE_RUN_SECONDS -1.0

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

// --- NkGame

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
NkUInt16 nkReactorGetFullNeighborsOf(NkUInt16 row, NkUInt16 col);

/// @brief Get the number of orthogonal (up, down, left, right) neighbors of a cell
NkUInt16 nkReactorGetOrthoNeighborsOf(NkUInt16 row, NkUInt16 col);

/// @brief Free all allocated memory for the reactor grid
NkVoid nkUninitNkReactor();

/// @brief Checks if the locations [row] and [col] are within the bounds.
NkBool nkIsValidLocInReactor(NkUInt16 row, NkUInt16 col);

/// @brief Copies the data of the given tile into the reactor grid at the given row and column
NkVoid nkReactorSet(NkUInt16 row, NkUInt16 col, NkTile other);

#endif