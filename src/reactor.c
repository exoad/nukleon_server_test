#include "nukleon_public.h"
#include <stdlib.h>

NkGame nkGameInstance = { 0 };

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
    if(!nkGameInstance.reactor)
    {
        NK_PANIC("Reactor pointer not initialized!");
    }
    if(row <= 0 || col <= 0 || row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
    }
    nkGameInstance.reactor[row][col] = other;
}

NkTile* nkReactorGet(NkInt16 row, NkInt16 col)
{
    if(!nkGameInstance.reactor)
    {
        NK_PANIC("Reactor pointer not initialized!");
    }
    if(row <= 0 || col <= 0 || row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
    }
    return &nkGameInstance.reactor[row][col];
}

NkVoid nkInitNkReactor(NkInt16 width, NkInt16 height)
{
    if(width <= 0 || height <= 0)
    {
        NK_PANICF("The width and height parameters cannot be <= 0. [%d, %d]", width, height);
        return;
    }
    if (nkGameInstance.reactor)
    {
        return; // already created
    }
    NkSize H = (NkSize) height;
    NkSize W = (NkSize) width;
    nkGameInstance.reactor = (NkTile**) malloc(H * sizeof *(nkGameInstance.reactor));
    if(!nkGameInstance.reactor)
    {
        NK_PANICF("%s", "Failed to allocate reactor row pointer array.");
        return;
    }
    for(NkInt16 y = 0; y < height; y++)
    {
        nkGameInstance.totalHeat = NK_REACTOR_DEFAULT_START_HEAT;
        nkGameInstance.maxHeat = NK_REACTOR_DEFAULT_MAX_HEAT;
        nkGameInstance. reactor[y] = (NkTile*) malloc(W * sizeof *(nkGameInstance.reactor[y]));
        if(!nkGameInstance.reactor[y])
        {
            NK_PANICF("Failed to allocate reactor row %d.", y);
            // cleanup previous
            for(NkInt16 i = 0; i < y; i++)
            {
                free(nkGameInstance.reactor[i]);
            }
            nkGameInstance = (NkGame) { 0 };
            return;
        }
        for(NkInt16 x = 0; x < width; x++)
        {
            NkTile* t = &(nkGameInstance.reactor[y][x]);
            t->id = kNkAirComponent->id;
            t->tier = 0;
            t->containedHeat = 0.0f;
            t->power = 0.0f;
            t->health = -1.0f;
            t->active = false;
            t->custom1 = 0;
            t->custom2 = 0;
        }
    }
    _height = height;
    _width  = width;
}

NkVoid nkUninitNkReactor()
{
    if(!nkGameInstance.reactor)
    {
        return;
    }
    for(NkInt16 y = 0; y < _height; y++)
    {
        free(nkGameInstance.reactor[y]);
    }
    nkGameInstance = (NkGame) { 0 };
    _width = _height = 0;
}