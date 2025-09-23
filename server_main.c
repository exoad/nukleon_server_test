#include "nukleon_public.h"

#define REACTOR_WIDTH ((NkInt16) 20)
#define REACTOR_HEIGHT ((NkInt16) 10)

NkInt32 main(NkVoid)
{
    nkInitTime();
    nkInitItemsDefinition();
    NK_PRINTLN("%s", "Hello World!");
    nkInitNkReactor(REACTOR_WIDTH, REACTOR_HEIGHT);
    NkTile tile = (NkTile) { .id = NK_CID(NK_COMPONENT_SINGLE_FUEL_CELL, NK_SINGLE_URANIUM_CELL), .active = true };
    // nkUpgradeCellComponent(&tile);
    // nkUpgradeCellComponent(&tile);
    // nkUpgradeCellComponent(&tile);
    nkReactorSet(4, 3, tile);
    nkGameLoop();
    nkUninitNkReactor();
    NK_PRINTLN("%s", "Bye bye.");
    return 0;
}