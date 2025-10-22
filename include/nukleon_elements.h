#ifndef NUKLEON_ELEMENTS_H
#define NUKLEON_ELEMENTS_H

#include "nukleon_types.h"

typedef struct NkTile NkTile;

typedef enum
{
    NK_COMPONENT_FLAG_USER_REMOVABLE = 1u << 0,
    NK_COMPONENT_FLAG_USER_PLACEABLE = 1u << 1
} NkComponentFlags;

#define NK_COMPONENT_INFINITE_HEALTH -1
#define nkparam_health
#define nkparam_base_heat
#define nkparam_base_power
#define nkparam_base_price
#define nkparam_upgrade_to
#define nkparam_upgrade_fx
#define nkparam_name
#define nkparam_flags
#define nkparam_id
#define nkparam_category
#define nkparam_symbol
#define nkparam_custom1
#define nkparam_custom2

Void nkUpgradeCellComponent(NkTile* tile);
Void nkNoUpGradeFx(NkTile* tile);

// TODO: introduce only 1 or 2 macros instead of the previous four. implicit/unused fields should not be hidden.
#define define_base_component(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_, custom1_, custom2_) sym_,

#define define_component(cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_, custom1_, custom2_) define_base_component(sym_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_, custom1_, custom2_)

typedef enum
{
#include "../assets/components/internal.def"
    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_INTERNAL_COMPONENT_COUNT // DO NOT USE FOR GAME COMPONENT REPRESENTATION
} NkInternalComponentSymbol;

typedef enum
{
#include "../assets/components/vents.def"
    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_VENT_COUNT // DO NOT USE FOR GAME COMPONENT REPRESENTATION
} NkVentSymbol;

typedef enum
{
#include "../assets/components/reactor_platings.def"
    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_REACTOR_PLATING_COUNT // DO NOT USE FOR GAME COMPONENT REPRESENTATION
} NkReactorPlatingSymbol;

typedef enum
{
#include "../assets/components/fuel_cell.def"
    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_FUEL_CELL_FUEL_CELL_COUNT // DO NOT USE FOR GAME COMPONENT REPRESENTATION
} NkSingleFuelCellSymbol;
#undef define_component
#undef define_base_component

#undef nkparam_health
#undef nkparam_base_heat
#undef nkparam_base_power
#undef nkparam_base_price
#undef nkparam_upgrade_to
#undef nkparam_upgrade_fx
#undef nkparam_name
#undef nkparam_flags
#undef nkparam_id
#undef nkparam_custom1
#undef nkparam_custom2
#undef nkparam_category
#undef nkparam_symbol

typedef enum NkComponentCategory
{
    NK_COMPONENT_INTERNAL = 0,
    NK_COMPONENT_FUEL_CELL,
    NK_COMPONENT_VENTS,
    NK_COMPONENT_PLATING,

    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_COMPONENT_CATEGORIES_COUNT // DO NOT USE FOR GAME COMPONENT REPRESENTATION
} NkComponentCategory;

extern const Int8* const gNkComponentCategoryNamesTable[NK_COMPONENT_CATEGORIES_COUNT];

typedef struct NkComponentIdentifier
{
    NkComponentCategory category;
    UInt16 id;
} NkComponentIdentifier;

/// @brief Checks if the two component identifiers are the same
__nk_inline Bool nkComponentIdentifierEquals(NkComponentIdentifier* a, NkComponentIdentifier* b)
{
    return a->id == b->id && a->category == b->category;
}

typedef struct NkTile
{
    NkComponentIdentifier id;
    UInt16 tier;
    Float32 generatedHeat; // this tick
    Float32 generatedPower; // this tick
    Float32 containedHeat; // or stored heat
    Float32 maxHeat; // capacity (max heat that can be stored)
    Float32 power;
    Int32 health;
    Bool active; // whether something is working (that is actually counted in the game logic)
                   // activity of a tile is attributed to the followign parameters:
                   //   1. the health is <= 0
                   //   2. the containedHeat > maxHeat
                   //   3. the power is == 0
                   //   4. this tile becomes out of bounds
                   //   5. arbitrary reason (manually or automatically set for reason)
    Float32 lastTickHeat;
    Float32 lastTickPower;
    Float32 tickHeat; // current working heat var
} NkTile;

NkTile newNkTileWithDefaultsFromId(NkComponentIdentifier id);

typedef struct
{
    CharSeq name;
    NkComponentIdentifier id;
    Float32 heatOutput;
    Float32 powerOutput;
    Int32 health;
    Void (*upgradeFx)(NkTile*);
    Int32 upgradeToId;
    Float32 basePrice;
    Int32 flags;
    // flexible fields; define accordingly (see each component's definition page)
    Int64 custom1;
    Float32 custom2;
} NkComponent;

__nk_inline void nkComponentSetFlag(NkComponent* c, UInt32 flag)
{
    c->flags |= flag;
}

__nk_inline void nkComponentClearFlag(NkComponent* c, UInt32 flag)
{
    c->flags &= ~flag;
}

__nk_inline Bool nkComponentHasFlag(const NkComponent* c, UInt32 flag)
{
    return (c->flags & flag) != 0;
}

#define NK_COMPONENT_LEAF -1

typedef struct {
    NkComponent* array;
    Int32 count;
} NkComponentCategoryTable;

extern const NkComponentCategoryTable gNkComponentCategories[NK_COMPONENT_CATEGORIES_COUNT];
extern const NkComponent* kNkAirComponent;

Void nkInitItemsDefinition();

__nk_always_inline __nk_inline Bool nkIsCellId(NkComponentIdentifier id)
{
    return id.category == NK_COMPONENT_FUEL_CELL; // add for compacted fuel cells
}

__nk_always_inline __nk_inline Bool nkIsPlatingId(NkComponentIdentifier id)
{
    return id.category == NK_COMPONENT_PLATING;
}

__nk_always_inline __nk_inline Bool nkIsVentId(NkComponentIdentifier id)
{
    return id.category == NK_COMPONENT_VENTS;
}

__nk_always_inline __nk_inline Bool nkIsHotTile(NkTile* tile)
{
    return tile->active && tile->tickHeat > 0.f;
}

Void nkTileToAir(NkTile* tile);

NkComponent* nkFindComponentById(NkComponentIdentifier id);

#endif
