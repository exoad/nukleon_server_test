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
    NK_COMPONENT_INTERNAL = 0,
    NK_COMPONENT_SINGLE_FUEL_CELL,
    NK_COMPONENT_VENTS,
    NK_COMPONENT_PLATING,

    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_COMPONENT_CATEGORIES_COUNT // DO NOT USE
} NkComponentCategory;

typedef struct
{
    NkComponentCategory category;
    NkInt16 id;
} NkComponentIdentifier;

typedef struct
{
    NkString name;
    NkComponentIdentifier id;
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

NkVoid nkNoUpGradeFx(NkTile* tile);

NkVoid nkUpgradeCellComponent(NkTile* tile);

typedef enum
{
    // -- Internal Components
    NK_AIR = 0,
    NK_BARRIER,
    // -- Singular Power Cells
    NK_SINGLE_FUEL_CELL_URANIUM,
    NK_SINGLE_FUEL_CELL_THORIUM,
    NK_SINGLE_FUEL_CELL_PLUTONIUM,
    NK_SINGLE_FUEL_CELL_CURIUM,
    NK_SINGLE_FUEL_CELL_FERMIUM,
    NK_SINGLE_FUEL_CELL_QUANTONIUM,
    NK_SINGLE_FUEL_CELL_THRAXIUM,
    NK_SINGLE_FUEL_CELL_SOLYTRIUM,
    NK_SINGLE_FUEL_CELL_CATANIONIUM,
    NK_SINGLE_FUEL_CELL_NEUTRACITE,
    NK_SINGLE_FUEL_CELL_TACHYTRIUM,
    // -- Vents
    NK_VENT_BASIC,
    // -- Platings
    NK_PLATING_CONCRETE,
    NK_PLATING_TITANIUM,
    NK_PLATING_LEAD,
    NK_PLATING_STEEL,
    NK_PLATING_TUNGSTEN,
    NK_PLATING_LIQUID_LEAD,
    NK_PLATING_LIQUID_LITHIUM   ,

} NkComponentsList;
#define NK_COMPONENT_LEAF -1

extern const NkComponent* const gNkComponentRegistry[NK_COMPONENT_CATEGORIES_COUNT];

NkVoid nkTileToAir(NkTile* tile);

NkComponent* nkFindComponentById(NkInt32 id);

#endif