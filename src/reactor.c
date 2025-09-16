#include "nukleon_public.h"
#include <stdlib.h>

NkTile** reactor = null;

static int _height = -1;
static int _width = -1;

NkInt16 nkReactorGetWidth()
{
    return _width;
}

NkInt16 nkReactorGetHeight()
{
    return _height;
}

NkVoid nkReactorSet(NkInt16 row, NkInt16 col, NkTile other)
{
    if(!reactor)
    {
        NK_PANIC("Reactor pointer not initialized!");
    }
    if(row <= 0 || col <= 0 || row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
    }
    reactor[row][col] = other;
}

NkTile nkReactorGet(NkInt16 row, NkInt16 col)
{
    if(!reactor)
    {
        NK_PANIC("Reactor pointer not initialized!");
    }
    if(row <= 0 || col <= 0 || row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
    }
    return reactor[row][col];
}

NkVoid newNkReactor(NkInt16 width, NkInt16 height)
{
    if(width <= 0 || height <= 0)
    {
        NK_PANICF("The width and height parameters cannot be negative. [%d, %d]", width, height);
        return;
    }
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
            reactor[x][y].componentId = NK_EMPTY;
            
            // reactor[y][x].component = NK_COMPONENT_EMPTY;
            // reactor[y][x].heat = 0;
            // reactor[y][x].power = 0;
            // reactor[y][x].health = 100;
            // reactor[y][x].active = 0;
        }
    }
    _height = height;
    _width = width;
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