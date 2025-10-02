#include "nukleon_public.h"
#include "tigr.h"

#define REACTOR_WIDTH ((NkUInt16) 32)
#define REACTOR_HEIGHT ((NkUInt16) 24)

NkVoid testBuildRun()
{
#ifdef NK_TEST_BUILD
    {
        NkTile tile = (NkTile) { .id = (NkComponentIdentifier) { NK_COMPONENT_SINGLE_FUEL_CELL, NK_SINGLE_URANIUM_CELL }, .active = true };
        nkUpgradeCellComponent(&tile);
        NK_ASSERT(tile.id.id == NK_SINGLE_THORIUM_CELL , "Expected Thorium Cell");
        nkUpgradeCellComponent(&tile);
        NK_ASSERT(tile.id.id == NK_SINGLE_PLUTONIUM_CELL, "Expected Plutonium Cell");
    }
#endif
}

NkInt32 main()
{
    nkInitTime();
    nkInitItemsDefinition();
    // testBuildRun();
    nkInitSampler();
    nkInitNkReactor(REACTOR_WIDTH, REACTOR_HEIGHT);
    // nkUpgradeCellComponent(&tile);
    // nkUpgradeCellComponent(&tile);
    // nkUpgradeCellComponent(&tile);
#include "../assets/sc_start.def"
#include "../assets/scripts/sc_initial_load.def"
#include "../assets/sc_end.def"

    nkGameLoop();
    nkUninitNkReactor();
    nkUninitSampler();
    NK_PRINTLN("%s", "Bye bye.");
    // tigrFree(screen);
    return 0;
}