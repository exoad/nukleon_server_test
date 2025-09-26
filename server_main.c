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
    for(NkUInt16 i = 0; i < NK_COMPONENT_CATEGORIES_COUNT; i++)
    {
        NkComponentCategoryTable cat = gNkComponentCategories[(NkInt16) i];
        NK_PRINTLN("%d --  %s", i, gNkComponentCategoryNamesTable[i]);
        for(NkUInt16 j = 0; j < cat.count; j++)
        {
            NK_PRINTLN("\t%d = [%s]", cat.array[j].id.id, cat.array[j].name);
            NK_PRINTLN("\t\tHeat Out: %5.4f", cat.array[j].heatOutput);
            NK_PRINTLN("\t\tPower Out: %5.4f", cat.array[j].powerOutput);
        }
    }
    // Tigr* screen = tigrWindow(320, 240, "nukleon_server_test ", TIGR_AUTO);
    // while(!tigrClosed(screen))
    // {
    //     tigrClear(screen, tigrRGB(0x80, 0x90, 0xa0));
    //     tigrPrint(screen, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), "Hello, world.");
    //     tigrUpdate(screen);
    // }
    nkGameLoop();
    nkUninitNkReactor();
    nkUninitSampler();
    NK_PRINTLN("%s", "Bye bye.");
    // tigrFree(screen);
    return 0;
}