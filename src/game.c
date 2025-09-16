#include "nukleon_public.h"
#include <stdlib.h>

NkTile** reactor = null;

static int _height = -1;

NkVoid newNkReactor(NkInt16 width, NkInt16 height)
{
    if(reactor)
    {
        return;
    }
    reactor = malloc(height * sizeof(NkTile*));
    if(!reactor)
    {
        return;
    }
    for(NkInt16 y = 0; y < height; y++)
    {
        reactor[y] = malloc(width * sizeof(NkTile));
        if(!reactor[y])
        {
            for(NkInt16 i = 0; i < y; i++)
            {
                free(reactor[i]);
            }
            free(reactor);
            reactor = NULL;
            return;
        }
        for(NkInt16 x = 0; x < width; x++)
        {
            reactor[y][x].type   = NK_TILE_EMPTY;
            reactor[y][x].heat   = 0;
            reactor[y][x].power  = 0;
            reactor[y][x].health = 100;
            reactor[y][x].active = 0;
        }
    }
    _height = height;
}

NkVoid freeNkReactor()
{
    if(!reactor)
    {
        return;
    }
    for(NkInt16 y = 0; y < _height; y++)
    {
        free(reactor[y]);
    }
    free(reactor);
    reactor = NULL;
}