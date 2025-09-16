#include "nukleon_public.h"

#define REACTOR_WIDTH ((NkInt16) 44)
#define REACTOR_HEIGHT ((NkInt16) 32)

NkVoid printReactor()
{
    for(NkInt16 row = 0; row < REACTOR_HEIGHT; row++)
    {
        for(NkInt16 col = 0; col < REACTOR_WIDTH; col++)
        {
            // NK_PRINT("[%d]", reactor[row][col].component);
        }
        NK_PRINTLN();
    }
}

NkInt32 main(NkVoid)
{
    NK_PRINTLN("%s", "Hello World!");
    newNkReactor(REACTOR_WIDTH, REACTOR_HEIGHT);
    // nkReactorSet(10, 10, (NkTile) { .component = NK_COMPONENT_CELL, .health = 99 });
    printReactor();
    freeNkReactor();
    return 0;
}