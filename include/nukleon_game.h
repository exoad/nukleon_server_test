#ifndef NUKLEON_GAME_H
#define NUKLEON_GAME_H

#include "nukleon_types.h"
#include "nukleon_elements.h"

// --- NkGame

typedef struct
{
    NkTile** reactor;
    Float64 totalPower;
    Float64 maxPower;
    Float64 totalHeat;
    Float64 maxHeat;
    UInt16 rows;
    UInt16 cols;
    UInt16 capacityRows;
    UInt16 capacityCols;
    UInt64 tickIndex;
    UInt64 credits;
    Float32 naturalHeatRemoval; // per tick how much heat to remove
} NkGame;

extern NkGame gNkGameInstance;

UInt16 nkReactorGetWidth();

/// @brief For meltdown purposes
Void nkResetReactor();

UInt16 nkReactorGetHeight();

Void nkInitNkReactor(UInt16 width, UInt16 height);

/// @brief Get the number of all (including diagonal) neighbors of a cell that are of the same category
UInt16 nkReactorGetFullNeighborsOfCat(NkComponentCategory cat, UInt16 row, UInt16 col);

/// @brief Get the number of orthogonal (up, down, left, right) neighbors of a cell that are of exactly type [id]
UInt16 nkReactorGetOrthoNeighborsOf(NkComponentIdentifier id, UInt16 row, UInt16 col);

/// @brief Returns the number of orthogonal neighbors that are of type [cat]
UInt16 nkReactorGetOrthoNeighborsOfCat(NkComponentCategory cat, UInt16 row, UInt16 col);

typedef struct
{
    NkTile* north;
    NkTile* south;
    NkTile* east;
    NkTile* west;
} NkOrthoNeighborTiles;

/// @brief Returns a struct containing the pointers to the tiles themselves.
NkOrthoNeighborTiles nkReactorFindOrthoNeighborsOfCat(NkComponentCategory cat, UInt16 row, UInt16 col);

/// @brief Free all allocated memory for the reactor grid
Void nkUninitNkReactor();

/// @brief Checks if the locations [row] and [col] are within the bounds.
Bool nkIsValidLocInReactor(UInt16 row, UInt16 col);

/// @brief Copies the data of the given tile into the reactor grid at the given row and column
Void nkReactorSet(UInt16 row, UInt16 col, NkTile other);

#endif