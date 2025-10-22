#include "nukleon.h"
#include "tigr.h"

#define REACTOR_WIDTH ((UInt16) 32)
#define REACTOR_HEIGHT ((UInt16) 24)

Void testBuildRun()
{
#ifdef NK_TEST_BUILD
    {
        NkTile tile = (NkTile) { .id = (NkComponentIdentifier) { NK_COMPONENT_FUEL_CELL, NK_FUEL_CELL_URANIUM }, .active = true };
        nkUpgradeCellComponent(&tile);
        NK_ASSERT(tile.id.id == NK_FUEL_CELL_THORIUM, "Expected Thorium Cell");
        nkUpgradeCellComponent(&tile);
        NK_ASSERT(tile.id.id == NK_FUEL_CELL_PLUTONIUM, "Expected Plutonium Cell");
    }
#endif
}

Int32 main()
{
    nkInitTime();
    nkInitItemsDefinition();
    // testBuildRun();
    nkInitSampler();
    nkInitNkReactor(REACTOR_WIDTH, REACTOR_HEIGHT);
#define reactor_width nkReactorGetWidth()
#define reactor_height nkReactorGetHeight()
#define reactor_set(row, col, tile) nkReactorSet(row, col, tile);
#define new_tile(id) newNkTileWithDefaultsFromId(id)
#define new_cmp(cat, id) ((NkComponentIdentifier) { cat, id })
#define for_each_cell_in_reactor(row, col) for(Int16 row = 0; row < reactor_height; row++) for(Int16 col = 0; col < reactor_width; col++)
#define reactor_get(row, col) gNkGameInstance.reactor[row][col]
#define toggle_activity(row, col) gNkGameInstance.reactor[row][col].active = !gNkGameInstance.reactor[row][col].active;
#define is_active(row, col) gNkGameInstance.reactor[row][col].active
#define print(...) NK_PRINTLN(__VA_ARGS__);
#define component_get(id) nkFindComponentById(id)
#define __C__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#include "../assets/scripts/sc_initial_load.def"
#pragma GCC diagnostic pop
#undef reactor_width
#undef reactor_height
#undef reactor_set
#undef new_tile
#undef new_cmp
#undef for_each_cell_in_reactor
#undef reactor_get
#undef print
#undef component_get
#undef toggle_activity
#undef __C__
    nkGameLoop();
    nkUninitNkReactor();
    nkUninitSampler();
    NK_PRINTLN("%s", "Bye bye.");
    // tigrFree(screen);
    return 0;
}