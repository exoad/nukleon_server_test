#ifndef NUKLEON_ELEMENTS_H
#define NUKLEON_ELEMENTS_H

#include "nukleon_shared.h"

typedef struct NkTile NkTile;

typedef enum
{
    NK_COMPONENT_FLAG_USER_REMOVABLE = 1u << 0,
    NK_COMPONENT_FLAG_USER_PLACEABLE = 1u << 1
} NkComponentFlags;

#define NK_COMPONENT_INFINITE_HEALTH -1
#define nkparam_durability
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

// TODO: introduce only 1 or 2 macros instead of the previous four. implicit/unused fields should not be hidden.

#define component(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_) sym_ = id_,
#define component_fx(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_) sym_ = id_,
#define component_custom(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, custom1_, custom2_) sym_ = id_,
#define component_fx_custom(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_, custom1_, custom2_) sym_ = id_,
#define mod_component(cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_) sym_,
#define mod_component_fx(cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_) sym_,
#define mod_component_custom(cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, custom1_, custom2_) sym_,
#define mod_component_fx_custom(cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_, custom1_, custom2_) sym_,

NkVoid nkUpgradeCellComponent(NkTile* tile);

typedef enum
{
#include "../assets/components.def"
    // -- DO NOT USE --
    // there is no definition for this item and will probably crash
    NK_COMPONENTS_COUNT // DO NOT USE FOR GAME COMPONENT REPRESENTATION
} NkComponentsList;

#undef component
#undef component_fx
#undef nkparam_durability
#undef nkparam_base_heat
#undef nkparam_base_power
#undef nkparam_base_price
#undef nkparam_upgrade_to
#undef nkparam_upgrade_fx
#undef nkparam_name
#undef nkparam_flags
#undef nkparam_id
#undef mod_component
#undef mod_component_fx
#undef nkparam_category
#undef nkparam_symbol
#undef mod_component_custom
#undef nkparam_custom1
#undef nkparam_custom2
#undef mod_component_fx_custom
#undef component_custom
#undef component_fx_custom

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
    NkFloat32 health; // out of /100 for direct percentage value
    NkBool active; // whether something is working (that is actually counted in the game logic)
                   // activity of a tile is attributed to the followign parameters:
                   //   1. the health is <= 0
                   //   2. the containedHeat > maxHeat
                   //   3. the power is == 0
                   //   4. this tile becomes out of bounds
                   //   5. arbitrary reason (manually or automatically set for reason)
} NkTile;

NkTile newNkTileWithDefaultsFromId(NkComponentIdentifier id);

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
    // flexible fields; define accordingly (see each component's definition page)
    NkInt64 custom1;
    NkFloat32 custom2;
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

#define NK_COMPONENT_LEAF -1

typedef struct {
    NkComponent* array;
    NkInt32 count;
} NkComponentCategoryTable;

extern const NkComponentCategoryTable gNkComponentCategories[NK_COMPONENT_CATEGORIES_COUNT];
extern const NkComponent* kNkAirComponent;

NkVoid nkInitItemsDefinition();

NkBool nkIsCellId(NkComponentIdentifier id);

NkBool nkIsPlatingId(NkComponentIdentifier id);

NkVoid nkTileToAir(NkTile* tile);

NkComponent* nkFindComponentById(NkComponentIdentifier id);

#endif
