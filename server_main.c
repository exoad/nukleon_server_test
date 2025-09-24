#include "nukleon_public.h"

#define REACTOR_WIDTH ((NkUInt16) 20)
#define REACTOR_HEIGHT ((NkUInt16) 10)


NkVoid testBuildRun()
{
#ifdef NK_TEST_BUILD
    {
        NkTile tile = (NkTile) { .id = NK_CID(NK_COMPONENT_SINGLE_FUEL_CELL, NK_SINGLE_URANIUM_CELL), .active = true };
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
    testBuildRun();
    nkInitNkReactor(REACTOR_WIDTH, REACTOR_HEIGHT);
    NkTile tile = (NkTile) { .id = NK_CID(NK_COMPONENT_SINGLE_FUEL_CELL, NK_SINGLE_URANIUM_CELL), .active = true };
    // nkUpgradeCellComponent(&tile);
    // nkUpgradeCellComponent(&tile);
    // nkUpgradeCellComponent(&tile);
    nkReactorSet((NkLocation) { 4, 3 }, tile);
    nkGameLoop();
    // for(NkUInt16 i = 0; i < NK_COMPONENT_CATEGORIES_COUNT; i++)
    // {
    //     NkComponentCategoryTable cat = gNkComponentCategories[(NkInt16) i];
    //     NK_PRINTLN("%d --  %s", i, gNkComponentCategoryNamesTable[i]);
    //     for(NkUInt16 j = 0; j < cat.count; j++)
    //     {
    //         NK_PRINTLN("\t%d = [%s]", cat.array[j].id.id, cat.array[j].name);
    //     }
    // }
    nkUninitNkReactor();
    NK_PRINTLN("%s", "Bye bye.");
    return 0;
}