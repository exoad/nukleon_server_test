#include "nukleon_public.h"

#define REACTOR_WIDTH ((NkInt16) 44)
#define REACTOR_HEIGHT ((NkInt16) 32)

NkInt32 main(NkVoid) {
    NK_PRINT("%s", "Hello World!");
    newNkReactor(REACTOR_WIDTH, REACTOR_HEIGHT);
    for(NkInt16 row = 0; row < REACTOR_HEIGHT; row++)
    {
        for(NkInt16 col = 0; col < REACTOR_WIDTH; col++)
        {
            printf("[%d]", reactor[row][col].type);
        }
        printf("\n");
    }
}