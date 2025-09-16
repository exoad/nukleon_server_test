#ifndef NUKLEON_ELEMENTS_H
#define NUKLEON_ELEMENTS_H

#include "nukleon_shared.h"

typedef enum
{
    NK_COMPONENT_EMPTY,
    NK_COMPONENT_CELL,
} NkComponent;

typedef struct
{
    NkComponent component;
    NkInt16 tier;
    NkFloat32 heat;
    NkFloat32 power;
    NkFloat32 health;
    NkBool active;
    // flexible fields
    NkInt32 custom1;
    NkInt32 custom2;
} NkTile;

typedef struct
{
    NkString name;
    NkFloat32 heatOutput;
    NkFloat32 powerOutput;
    NkInt32 durability;
} NkComponentTier;

#endif