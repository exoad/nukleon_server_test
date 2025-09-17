#include "nukleon_public.h"

#define REACTOR_WIDTH ((NkInt16) 20)
#define REACTOR_HEIGHT ((NkInt16) 10)

NkVoid printReactor()
{
    for(NkInt16 row = 0; row < REACTOR_HEIGHT; row++)
    {
        for(NkInt16 col = 0; col < REACTOR_WIDTH; col++)
        {
            const NkInt32 v = nkGameInstance.reactor[row][col].componentId;
            if(v == 0)
            {
                NK_PRINT("%s", "[  ]");
            }
            else
            {
                NK_PRINT(nkGameInstance.reactor[row][col].active ? "[%da]" : "[%dd]", v);
            }
        }
        NK_PRINTLN();
    }
}

NkInt32 main(NkVoid)
{
    nkTimeInit();
    NK_PRINTLN("%s", "Hello World!");
    newNkReactor(REACTOR_WIDTH, REACTOR_HEIGHT);
    NkTile tile = (NkTile) { .componentId = NK_SINGLE_URANIUM_CELL, .active = true };
    nkUpgradeCellComponent(&tile);
    nkUpgradeCellComponent(&tile);
    nkUpgradeCellComponent(&tile);
    nkReactorSet(4, 3, tile);
    printReactor();
    freeNkReactor();
    NK_PRINTLN("%s", "Bye bye.");
    return 0;
}