#ifndef NUKLEON_GAME_H
#define NUKLEON_GAME_H

#include "nukleon_types.h"
#include "nukleon_elements.h"

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
    NkFloat32 naturalHeatRemoval; // per tick how much heat to remove
} NkGame;

extern NkGame gNkGameInstance;

NkUInt16 nkReactorGetWidth();

/// @brief For meltdown purposes
NkVoid nkResetReactor();

NkUInt16 nkReactorGetHeight();

NkVoid nkInitNkReactor(NkUInt16 width, NkUInt16 height);

/// @brief Get the number of all (including diagonal) neighbors of a cell that are of the same category
NkUInt16 nkReactorGetFullNeighborsOfCat(NkComponentCategory cat, NkUInt16 row, NkUInt16 col);

/// @brief Get the number of orthogonal (up, down, left, right) neighbors of a cell that are of exactly type [id]
NkUInt16 nkReactorGetOrthoNeighborsOf(NkComponentIdentifier id, NkUInt16 row, NkUInt16 col);

/// @brief Returns the number of orthogonal neighbors that are of type [cat]
NkUInt16 nkReactorGetOrthoNeighborsOfCat(NkComponentCategory cat, NkUInt16 row, NkUInt16 col);

typedef struct
{
    NkTile* north;
    NkTile* south;
    NkTile* east;
    NkTile* west;
} NkOrthoNeighborTiles;

/// @brief Returns a struct containing the pointers to the tiles themselves.
NkOrthoNeighborTiles nkReactorFindOrthoNeighborsOfCat(NkComponentCategory cat, NkUInt16 row, NkUInt16 col);

/// @brief Free all allocated memory for the reactor grid
NkVoid nkUninitNkReactor();

/// @brief Checks if the locations [row] and [col] are within the bounds.
NkBool nkIsValidLocInReactor(NkUInt16 row, NkUInt16 col);

/// @brief Copies the data of the given tile into the reactor grid at the given row and column
NkVoid nkReactorSet(NkUInt16 row, NkUInt16 col, NkTile other);

#endif