#include "nukleon_elements.h"
#include <string.h>

#define NK_SINGLE_FUEL_CELL_FLAGS (NK_COMPONENT_FLAG_USER_PLACEABLE | NK_COMPONENT_FLAG_USER_REMOVABLE)

/// Used for marking things with a properly defined update function
#define component_fx(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_, upgradeFx_) \
    [id_] = { name_, { cat_, id_ }, heat_, power_, dura_, upgradeFx_, upgrade_, price_, flags_ },
/// Used for defining simple items without an upgrade function or one that will be linked internally dynamically
#define component(id_, cat_, sym_, name_, heat_, power_, dura_, price_, flags_, upgrade_) \
    [id_] = { name_, { cat_, id_ }, heat_, power_, dura_, null, upgrade_, price_, flags_ },

static NkComponent _internalComponents[] = {
#include "../assets/components/internal.def"
};

static NkComponent _singleFuelCellComponents[] = {
#include "../assets/components/single_fuel_cell.def"
};

/// Master lookup table that defines and allocates all necessary resources for each category of component
#define _new_cat(cat) { .array = cat, .count = (NkInt32) (sizeof cat / sizeof *cat) }
#define _todo_cat() { .array = null, .count = (NkInt32) 0 }

/*extern*/ const NkComponentCategoryTables gNkComponentCategories[NK_COMPONENT_CATEGORIES_COUNT] = {
    [NK_COMPONENT_INTERNAL] = _new_cat(_internalComponents),
    [NK_COMPONENT_SINGLE_FUEL_CELL] = _new_cat(_singleFuelCellComponents),
    // not yet implemented!!
    [NK_COMPONENT_VENTS]  = _todo_cat(),
    [NK_COMPONENT_PLATING] = _todo_cat()
};

#undef _new_cat
#undef _todo_cat
#undef component
#undef component_fx

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

// NkComponent componentRegistry[] = {
//     // -- Internal components
//     {
//         NK_AIR,
//         .name = "Air",
//         .heatOutput = 0,
//         .powerOutput = 0,
//         .durability = -9999,
//         .flags = 0
//     },
//     {
//         NK_BARRIER,
//         .name = "Barrier",
//         .heatOutput = 0,
//         .powerOutput = 0,
//         .durability = -9999,
//         .flags = 0
//     },

//     // -- Singular Power Cells
//     {
//         NK_SINGLE_URANIUM_CELL,
//         .name = "Uranium Cell",
//         .heatOutput = 5,
//         .powerOutput = 10,
//         .durability = 100,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_THORIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_THORIUM_CELL,
//         .name = "Thorium Cell",
//         .heatOutput = 8,
//         .powerOutput = 20,
//         .durability = 150,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_PLUTONIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_PLUTONIUM_CELL,
//         .name = "Plutonium Cell",
//         .heatOutput = 15,
//         .powerOutput = 40,
//         .durability = 200,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_CURIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_CURIUM_CELL,
//         .name = "Curium Cell",
//         .heatOutput = 20,
//         .powerOutput = 55,
//         .durability = 220,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FERMIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FERMIUM_CELL,
//         .name = "Fermium Cell",
//         .heatOutput = 25,
//         .powerOutput = 70,
//         .durability = 240,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_QUANTONIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_QUANTONIUM_CELL,
//         .name = "Quantonium Cell",
//         .heatOutput = 30,
//         .powerOutput = 90,
//         .durability = 260,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_THRAXIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_THRAXIUM_CELL,
//         .name = "Thraxium Cell",
//         .heatOutput = 35,
//         .powerOutput = 110,
//         .durability = 280,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_SOLYTRIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_SOLYTRIUM_CELL,
//         .name = "Solytrium Cell",
//         .heatOutput = 40,
//         .powerOutput = 135,
//         .durability = 300,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_CATANIONIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_CATANIONIUM_CELL,
//         .name = "Catanionium Cell",
//         .heatOutput = 45,
//         .powerOutput = 160,
//         .durability = 320,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_NEUTRACITE_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_NEUTRACITE_CELL,
//         .name = "Neutracite Cell",
//         .heatOutput = 50,
//         .powerOutput = 190,
//         .durability = 340,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_TACHYTRIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_TACHYTRIUM_CELL,
//         .name = "Tachytrium Cell",
//         .heatOutput = 55,
//         .powerOutput = 220,
//         .durability = 360,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_PATCHNOTIUM_CELL,
//         .flags = NK_SINGLE_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_PATCHNOTIUM_CELL,
//         .name = "Patchnotium Cell",
//         .heatOutput = 60,
//         .powerOutput = 250,
//         .durability = 400,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_COMPONENT_LEAF,
//         .flags = NK_SINGLE_CELL_FLAGS
//     }
// };

// NkComponent* nkFindComponentById(NkInt32 id)
// {
//     NkSize count = sizeof(componentRegistry) / sizeof(componentRegistry[0]);
//     for(NkSize i = 0; i < count; i++)
//     {
//         if(componentRegistry[i].id == id)
//         {
//             return &componentRegistry[i];
//         }
//     }
//     return null;
// }

// NkVoid nkUpgradeCellComponent(NkTile* tile)
// {
//     if(!tile)
//     {
//         return;
//     }
//     const NkComponent* current = nkFindComponentById(tile->componentId);
//     if(current->upgradeToId == NK_COMPONENT_LEAF)
//     {
//         // NK_PRINTLN("ERROR: component %d is already at max level", tile->componentId);
//         return;
//     }
//     if(!current)
//     {
//         NK_PRINTLN("ERROR: component id %d not found", tile->componentId);
//         return;
//     }
//     const NkComponent* upgraded = nkFindComponentById(current->upgradeToId);
//     if(!upgraded)
//     {
//         return;
//     }
//     tile->componentId = upgraded->id;
//     tile->containedHeat = upgraded->heatOutput;
//     tile->power = upgraded->powerOutput;
//     tile->health = (NkFloat32)upgraded->durability;
//     tile->tier += 1;
//     tile->custom1 = 0;
//     tile->custom2 = 0;
//     // no auto chain upgrades for now
//     // if(upgraded->upgradeFx)
//     // {
//     //     upgraded->upgradeFx(tile);
//     // }
// }

// NkVoid nkTileToAir(NkTile* tile)
// {
//     memset(tile, 0, sizeof(*tile));
//     tile->componentId = NK_AIR;
//     tile->tier = 1;
//     tile->maxHeat = 1.0;
//     tile->health = 100.0f;
//     tile->active = false;

// }