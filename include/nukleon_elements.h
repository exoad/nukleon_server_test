#ifndef NUKLEON_ELEMENTS_H
#define NUKLEON_ELEMENTS_H

#include "nukleon_shared.h"

typedef struct
{
    NkInt32 componentId;
    NkInt16 tier;
    NkFloat32 generatedHeat; // this tick
    NkFloat32 generatedPower; // this tick
    NkFloat32 containedHeat; // or stored heat
    NkFloat32 maxHeat; // capacity (max heat that can be stored)
    NkFloat32 power;
    NkFloat32 health;
    NkBool active; // whether something is working (that is actually counted in the game logic)
                   // activity of a tile is attributed to the followign parameters:
                   //   1. the health is <= 0
                   //   2. the containedHeat > maxHeat
                   //   3. the power is == 0
                   //   4. this tile becomes out of bounds
                   //   5. arbitrary reason (manually or automatically set for reason)
    // flexible fields
    NkInt32 custom1;
    NkInt32 custom2;
} NkTile;

typedef enum
{
    NK_COMPONENT_FLAG_USER_REMOVABLE = 1u << 0,
    NK_COMPONENT_FLAG_USER_PLACEABLE = 1u << 1
} NkComponentFlags;

typedef enum
{
    NK_COMPONENT_CAT_INTERNAL = 0,
    NK_COMPONENT_CAT_SINGLE_FUEL_CELL,
    NK_COMPONENT_CAT_COUNT // only used for counting total
} NkComponentCategory;

#define NK_COMPONENT_LEAF -1

typedef enum {
#define component(cat, sym, name, heat, power, dura, price, flags, upgrade) sym,
#include "../assets/components.def"
#undef component
    NK_COMPONENT_COUNT
} NkComponentId;

typedef struct
{
    NkInt32 id;
    NkString name;
    NkFloat32 heatOutput;
    NkFloat32 powerOutput;
    NkInt32 durability;
    NkVoid (*upgradeFx)(NkTile*);
    NkInt32 upgradeToId;
    NkFloat32 basePrice;
    NkInt32 flags;
} NkComponent;

static inline void nkComponentSetFlag(NkComponent* c, NkUInt32 flag)
{
    c->flags |= flag;
}

static inline void nkComponentClearFlag(NkComponent* c, NkUInt32 flag)
{
    c->flags &= ~flag;
}

static inline NkBool nkComponentHasFlag(const NkComponent* c, NkUInt32 flag)
{
    return (c->flags & flag) != 0;
}


NkVoid nkUpgradeCellComponent(NkTile* tile);

extern NkComponent componentRegistry[NK_COMPONENT_COUNT];

NkVoid nkTileToAir(NkTile* tile);

NkComponent* nkFindComponentById(NkInt32 id);

#endif