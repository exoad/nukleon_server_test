#ifndef NUKLEON_ELEMENTS_H
#define NUKLEON_ELEMENTS_H

#include "nukleon_shared.h"

typedef enum
{
    NK_TILE_EMPTY,
    NK_TILE_CELL
} NkTileType;

typedef struct
{
    NkTileType type;
    NkInt32 heat;
    NkInt32 power;
    NkInt32 health;
    NkBool active;
} NkTile;

#endif