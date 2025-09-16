#ifndef NUKLEON_ELEMENTS_H
#define NUKLEON_ELEMENTS_H

#include "nukleon_shared.h"

typedef struct
{
    NkInt32 componentId;
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
    NkInt32 id;
    NkString name;
    NkFloat32 heatOutput;
    NkFloat32 powerOutput;
    NkInt32 durability;
    NkVoid (*updateFunc)(NkTile*);
    NkInt32 upgradeToId;
} NkComponent;

NkVoid nkUpradeCellComponent(NkTile* tile);

typedef enum
{
    NK_EMPTY = 0,
    NK_URANIUM_CELL,
    NK_THORIUM_CELL,
    NK_PLUTONIUM_CELL
} NkComponentsList;

static const NkComponent componentRegistry[] = {
    { NK_URANIUM_CELL, "Uranium Cell", 5, 10, 100, nkUpradeCellComponent, NK_THORIUM_CELL },
    { NK_THORIUM_CELL, "Thorium Cell", 8, 20, 150, nkUpradeCellComponent, NK_PLUTONIUM_CELL },
    { NK_PLUTONIUM_CELL, "Plutonium Cell", 15, 40, 200, nkUpradeCellComponent, -1 },
};

#endif