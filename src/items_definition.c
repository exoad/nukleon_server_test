#include "nukleon_elements.h"
#include <string.h>

#define NK_SINGLE_FUEL_CELL_FLAGS (NK_COMPONENT_FLAG_USER_PLACEABLE | NK_COMPONENT_FLAG_USER_REMOVABLE)
/// Used for marking things with a properly defined update function
#define component_fx( \
    id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_ \
) \
    [id_] = { \
        name_, \
        { cat_, id_ }, \
        heat_, \
        power_, \
        dura_, \
        upgradeFx_, \
        upgrade_, \
        price_, \
        flags_, \
        0, \
        0.0f \
    },

#define component( \
    id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_ \
) \
    [id_] = { \
        name_, \
        { cat_, id_ }, \
        heat_, \
        power_, \
        dura_, \
        null, \
        upgrade_, \
        price_, \
        flags_, \
        0, \
        0.0f \
    },

#define mod_component( \
    cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_ \
) \
    { \
        name_, \
        { cat_, sym_ }, \
        heat_, \
        power_, \
        dura_, \
        null, \
        upgrade_, \
        price_, \
        flags_, \
        0, \
        0.0f \
    },

#define mod_component_fx( \
    cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_ \
) \
    component_fx( \
        sym_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_ \
    )

#define mod_component_custom( \
    cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, custom1_, custom2_ \
) \
    { \
        name_, \
        { cat_, sym_ }, \
        heat_, \
        power_, \
        dura_, \
        null, \
        upgrade_, \
        price_, \
        flags_, \
        custom1_, \
        custom2_ \
    },

#define mod_component_fx_custom( \
    cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_, custom1_, custom2_ \
) \
    { \
        name_, \
        { cat_, sym_ }, \
        heat_, \
        power_, \
        dura_, \
        upgradeFx_, \
        upgrade_, \
        price_, \
        flags_, \
        custom1_, \
        custom2_ \
    },

#define component_custom( \
    id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, custom1_, custom2_ \
) \
    [id_] = { \
        name_, \
        { cat_, id_ }, \
        heat_, \
        power_, \
        dura_, \
        null, \
        upgrade_, \
        price_, \
        flags_, \
        custom1_, \
        custom2_ \
    },

#define component_fx_custom( \
    id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_, custom1_, custom2_ \
) \
    [id_] = { \
        name_, \
        { cat_, id_ }, \
        heat_, \
        power_, \
        dura_, \
        upgradeFx_, \
        upgrade_, \
        price_, \
        flags_, \
        custom1_, \
        custom2_ \
    },
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

static NkComponent _internalComponents[] = {
#include "../assets/components/internal.def"
};

static NkComponent _singleFuelCellComponents[] = {
#include "../assets/components/single_fuel_cell.def"
};

static NkComponent _reactorPlatingComponents[] = {
#include "../assets/components/reactor_platings.def"
};

static NkComponent _ventComponents[] = {
#include "../assets/components/vents.def"
};

/// Master lookup table that defines and allocates all necessary resources for each category of component
#define _new_cat(cat) { .array = cat, .count = (NkInt32) (sizeof cat / sizeof *cat) }
#define _todo_cat() { .array = null, .count = (NkInt32) 0 }

/*extern*/ const NkComponentCategoryTable gNkComponentCategories[NK_COMPONENT_CATEGORIES_COUNT] = {
    [NK_COMPONENT_INTERNAL] = _new_cat(_internalComponents),
    [NK_COMPONENT_SINGLE_FUEL_CELL] = _new_cat(_singleFuelCellComponents),
    // not yet implemented!!
    [NK_COMPONENT_VENTS]  = _new_cat(_ventComponents),
    [NK_COMPONENT_PLATING] = _new_cat(_reactorPlatingComponents),
};

#undef _new_cat
#undef _todo_cat
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
#undef nkparam_category
#undef nkparam_symbol
#undef mod_component
#undef mod_component_fx
#undef mod_component_custom
#undef nkparam_custom1
#undef nkparam_custom2
#undef mod_component_fx_custom
#undef component_custom
#undef component_fx_custom

/*extern*/ const NkInt8* const gNkComponentCategoryNamesTable[NK_COMPONENT_CATEGORIES_COUNT] = {
#include "../assets/strings/component_categories.def"
};

/*extern-late*/ const NkComponent* kNkAirComponent;

NkVoid nkInitItemsDefinition()
{
    kNkAirComponent = &_internalComponents[0]; // we can gurantee for internal items
}

NkComponent* nkFindComponentById(NkComponentIdentifier id)
{
    if((NkUInt32) id.category >= (NkUInt32) NK_COMPONENT_CATEGORIES_COUNT || id.id > gNkComponentCategories[id.category].count)
    {
        return null;
    }
    return &gNkComponentCategories[id.category].array[id.id];
}

NkVoid nkUpgradeCellComponent(NkTile* tile)
{
    if(!tile)
    {
        return;
    }
    NkComponent* current = nkFindComponentById(tile->id);
    if(!current)
    {
        return;
    }
    if(current->upgradeToId == NK_COMPONENT_LEAF)
    {
        return;
    }
    NkComponent* upgraded = nkFindComponentById((NkComponentIdentifier) { current->id.category, current->upgradeToId });
    if(!upgraded)
    {
        return;
    }
    tile->id = upgraded->id;
    tile->containedHeat = upgraded->heatOutput;
    tile->power = upgraded->powerOutput;
    tile->health = (NkFloat32)upgraded->durability;
    tile->tier += 1;
    // no auto chain upgrades for now
    // if(upgraded->upgradeFx)
    // {
    //     upgraded->upgradeFx(tile);
    // }
}

NkVoid nkNoUpGradeFx(NkTile* tile)
{
    (NkVoid) tile;
}

NkVoid nkTileToAir(NkTile* tile)
{
    if(!tile)
    {
        return;
    }
    tile->id = kNkAirComponent->id;
    tile->tier = 0;
    tile->generatedHeat = 0.0f;
    tile->generatedPower = 0.0f;
    tile->containedHeat = 0.0f;
    tile->maxHeat = 0.0f;
    tile->power = 0.0f;
    tile->health = -1.0f;
    tile->active = false;
}

NkBool nkIsCellId(NkComponentIdentifier id)
{
    return id.category == NK_COMPONENT_SINGLE_FUEL_CELL; // add for compacted fuel cells
}

NkBool nkIsPlatingId(NkComponentIdentifier id)
{
    return id.category == NK_COMPONENT_PLATING;
}

NkTile newNkTileWithDefaultsFromId(NkComponentIdentifier id)
{
    return (NkTile) {
        .id = id,
        .tier = 0,
        .generatedHeat = 0,
        .generatedPower = 0,
        .containedHeat = 0,
        .maxHeat = 0,
        .power = 0,
        .health = 100.0f, // we start with 100% health
        .active = true,
    };
}
