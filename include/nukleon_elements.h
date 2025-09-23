#ifndef NUKLEON_ELEMENTS_H
#define NUKLEON_ELEMENTS_H

#include "nukleon_shared.h"

typedef struct NkTile NkTile;

typedef enum
{
    NK_COMPONENT_FLAG_USER_REMOVABLE = 1u << 0,
    NK_COMPONENT_FLAG_USER_PLACEABLE = 1u << 1
} NkComponentFlags;

#define component(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_) sym_,
#define component_fx(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_) sym_,

NkVoid nkUpgradeCellComponent(NkTile* tile);

typedef enum
{
    #include "../assets/components/internal.def"
    #include "../assets/components/single_fuel_cell.def"
} NkComponentsList;

#undef component
#undef component_fx

typedef enum NkComponentCategory
{
    NK_COMPONENT_INTERNAL = 0,
    NK_COMPONENT_SINGLE_FUEL_CELL,
    NK_COMPONENT_VENTS,
    NK_COMPONENT_PLATING,

    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_COMPONENT_CATEGORIES_COUNT // DO NOT USE FOR GAME COMPONENT REPRESENTATION
} NkComponentCategory;

extern const NkInt8* const gNkComponentCategoryNamesTable[NK_COMPONENT_CATEGORIES_COUNT];

typedef struct NkComponentIdentifier
{
    NkComponentCategory category;
    NkUInt16 id;
} NkComponentIdentifier;

typedef struct NkTile
{
    NkComponentIdentifier id;
    NkUInt16 tier;
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

#define NK_CID(cat, itemid) { .category = cat, .id = itemid }

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

// typedef enum
// {
//     // -- Internal Components
//     NK_AIR = 0,
//     NK_BARRIER,
//     // -- Singular Power Cells
//     NK_SINGLE_FUEL_CELL_URANIUM,
//     NK_SINGLE_FUEL_CELL_THORIUM,
//     NK_SINGLE_FUEL_CELL_PLUTONIUM,
//     NK_SINGLE_FUEL_CELL_CURIUM,
//     NK_SINGLE_FUEL_CELL_FERMIUM,
//     NK_SINGLE_FUEL_CELL_QUANTONIUM,
//     NK_SINGLE_FUEL_CELL_THRAXIUM,
//     NK_SINGLE_FUEL_CELL_SOLYTRIUM,
//     NK_SINGLE_FUEL_CELL_CATANIONIUM,
//     NK_SINGLE_FUEL_CELL_NEUTRACITE,
//     NK_SINGLE_FUEL_CELL_TACHYTRIUM,
//     // -- Vents
//     NK_VENT_BASIC,
//     // -- Platings
//     NK_PLATING_CONCRETE,
//     NK_PLATING_TITANIUM,
//     NK_PLATING_LEAD,
//     NK_PLATING_STEEL,
//     NK_PLATING_TUNGSTEN,
//     NK_PLATING_LIQUID_LEAD,
//     NK_PLATING_LIQUID_LITHIUM   ,

// } NkComponentsList;
#define NK_COMPONENT_LEAF -1

typedef struct {
    NkComponent* array;
    NkInt32 count;
} NkComponentCategoryTable;

extern const NkComponentCategoryTable gNkComponentCategories[NK_COMPONENT_CATEGORIES_COUNT];
extern const NkComponent* kNkAirComponent;

NkVoid nkInitItemsDefinition();

NkVoid nkTileToAir(NkTile* tile);

NkComponent* nkFindComponentById(NkComponentIdentifier id);

#endif