#include "nukleon_elements.h"
#include <string.h>

#define NK_SINGLE_FUEL_CELL_FLAGS (NK_COMPONENT_FLAG_USER_PLACEABLE | NK_COMPONENT_FLAG_USER_REMOVABLE)

NkVoid nkNoUpGradeFx(NkTile* tile)
{
    (NkVoid) tile;
}

static const NkComponent nkComponentCatPlating[] = {
    
};

const NkComponent* const gNkComponentRegistry[NK_COMPONENT_CATEGORIES_COUNT] = {
    [NK_COMPONENT_PLATING] = nkComponentCatPlating
};


// NkComponent componentRegistry[] = {
//     // -- Internal components
//     {
//         NK_AIR,
//         .name = "Air",
//         .heatOutput = 0,
//         .powerOutput = 0,
//         .durability = -9999,
//         .flags = 0,
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
//         NK_SINGLE_FUEL_CELL_URANIUM,
//         .name = "Uranium Cell",
//         .heatOutput = 5,
//         .powerOutput = 10,
//         .durability = 100,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_THORIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_THORIUM,
//         .name = "Thorium Cell",
//         .heatOutput = 8,
//         .powerOutput = 20,
//         .durability = 150,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_PLUTONIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_PLUTONIUM,
//         .name = "Plutonium Cell",
//         .heatOutput = 15,
//         .powerOutput = 40,
//         .durability = 200,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_CURIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_CURIUM,
//         .name = "Curium Cell",
//         .heatOutput = 20,
//         .powerOutput = 55,
//         .durability = 220,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_FERMIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_FERMIUM,
//         .name = "Fermium Cell",
//         .heatOutput = 25,
//         .powerOutput = 70,
//         .durability = 240,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_QUANTONIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_QUANTONIUM,
//         .name = "Quantonium Cell",
//         .heatOutput = 30,
//         .powerOutput = 90,
//         .durability = 260,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_THRAXIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_THRAXIUM,
//         .name = "Thraxium Cell",
//         .heatOutput = 35,
//         .powerOutput = 110,
//         .durability = 280,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_SOLYTRIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_SOLYTRIUM,
//         .name = "Solytrium Cell",
//         .heatOutput = 40,
//         .powerOutput = 135,
//         .durability = 300,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_CATANIONIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_CATANIONIUM,
//         .name = "Catanionium Cell",
//         .heatOutput = 45,
//         .powerOutput = 160,
//         .durability = 320,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_NEUTRACITE,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_NEUTRACITE,
//         .name = "Neutracite Cell",
//         .heatOutput = 50,
//         .powerOutput = 190,
//         .durability = 340,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_TACHYTRIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
//     {
//         NK_SINGLE_FUEL_CELL_TACHYTRIUM,
//         .name = "Tachytrium Cell",
//         .heatOutput = 55,
//         .powerOutput = 220,
//         .durability = 360,
//         .upgradeFx = nkUpgradeCellComponent,
//         .upgradeToId = NK_SINGLE_FUEL_CELL_PATCHNOTIUM,
//         .flags = NK_SINGLE_FUEL_CELL_FLAGS
//     },
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