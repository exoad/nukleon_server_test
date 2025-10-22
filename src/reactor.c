#include "nukleon.h"
#include <stdlib.h>

NkGame gNkGameInstance = { 0 };

static UInt16 _height = -1;
static UInt16 _width = -1;

__nk_always_inline __nk_inline Void _ensureReactorInitialized()
{
    if(!gNkGameInstance.reactor)
    {
        NK_PANIC("Game Instance's reactor is null!");
        return;
    }
}

__nk_always_inline __nk_hot __nk_inline Void _checkReactorPos(UInt16 row, UInt16 col)
{
    if(row <= 0 || col <= 0 || row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
        return;
    }
}

UInt16 nkReactorGetWidth()
{
    return _width;
}

UInt16 nkReactorGetHeight()
{
    return _height;
}

Void nkReactorSet(UInt16 row, UInt16 col, NkTile other)
{
    _ensureReactorInitialized();
    if(row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
    }
    gNkGameInstance.reactor[row][col] = other;
}

Bool nkIsValidLocInReactor(UInt16 row, UInt16 col)
{
    return col < nkReactorGetWidth() && row < nkReactorGetHeight();
}

Void nkInitNkReactor(UInt16 width, UInt16 height)
{
    if(width <= 0 || height <= 0)
    {
        NK_PANICF("The width and height parameters cannot be <= 0. [%d, %d]", width, height);
        return;
    }
    if (gNkGameInstance.reactor)
    {
        return; // already created
    }
    Size H = (Size) height;
    Size W = (Size) width;
    gNkGameInstance.reactor = (NkTile**) malloc(H * sizeof *(gNkGameInstance.reactor));
    if(!gNkGameInstance.reactor)
    {
        NK_PANICF("%s", "Failed to allocate reactor row pointer array.");
        return;
    }
    for(UInt16 y = 0; y < height; y++)
    {
        gNkGameInstance.totalHeat = 0.0f;
        gNkGameInstance.maxHeat = NK_RULE_GAME_REACTOR_STARTING_MAX_HEAT;
        gNkGameInstance.maxPower = NK_RULE_GAME_REACTOR_STARTING_MAX_POWER;
        gNkGameInstance.naturalHeatRemoval = NK_RULE_GAME_REACTOR_STARTING_SELF_DISSIPATION_HEAT_RATE;
        gNkGameInstance.reactor[y] = (NkTile*) malloc(W * sizeof *(gNkGameInstance.reactor[y]));
        if(!gNkGameInstance.reactor[y])
        {
            NK_PANICF("Failed to allocate reactor row %d.", y);
            // cleanup previous
            for(UInt16 i = 0; i < y; i++)
            {
                free(gNkGameInstance.reactor[i]);
            }
            gNkGameInstance = (NkGame) { 0 };
            return;
        }
        for(UInt16 x = 0; x < width; x++)
        {
            NkTile* t = &(gNkGameInstance.reactor[y][x]);
            t->id = kNkAirComponent->id;
            t->tier = 0;
            t->containedHeat = 0.0f;
            t->power = 0.0f;
            t->health = -1.0f;
            t->active = false;
        }
    }
    _height = height;
    _width  = width;
}

Void nkUninitNkReactor()
{
    if(!gNkGameInstance.reactor)
    {
        return;
    }
    for(UInt16 y = 0; y < _height; y++)
    {
        free(gNkGameInstance.reactor[y]);
    }
    gNkGameInstance = (NkGame) { 0 };
    _width = _height = 0;
}

UInt16 nkReactorGetFullNeighborsOfCat(NkComponentCategory cat, UInt16 row, UInt16 col)
{
    _ensureReactorInitialized();
    _checkReactorPos(row, col);
    UInt16 count = 0;
    for(Int16 dy = -1; dy <= 1; dy++)
    {
        for(Int16 dx = -1; dx <= 1; dx++)
        {
            if(dy == 0 && dx == 0)
            {
                continue;
            }
            Int16 ny = (Int16) row + dy;
            Int16 nx = (Int16) col + dx;
            if(ny > 0 && ny <= _height && nx > 0 && nx <= _width)
            {
                if(gNkGameInstance.reactor[ny][nx].id.category == cat)
                {
                    count++;
                }
            }
        }
    }
    return count;
}

static const Int16 _directions[4][2] = {
    { -1, 0 }, // up
    { 1, 0 },  // down
    { 0, -1 }, // left
    { 0, 1 }   // right
};
UInt16 nkReactorGetOrthoNeighborsOf(NkComponentIdentifier id, UInt16 row, UInt16 col)
{
    _ensureReactorInitialized();
    _checkReactorPos(row, col);
    UInt16 count = 0;
    for(Int16 i = 0; i < 4; i++)
    {
        const Int16 ny = (Int16) row + _directions[i][0];
        const Int16 nx = (Int16) col + _directions[i][1];
        if(ny > 0 && ny <= _height && nx > 0 && nx <= _width)
        {
            if(nkComponentIdentifierEquals(&gNkGameInstance.reactor[ny][nx].id, &id))
            {
                count++;
            }
        }
    }
    return count;
}

UInt16 nkReactorGetOrthoNeighborsOfCat(NkComponentCategory cat, UInt16 row, UInt16 col)
{
    _ensureReactorInitialized();
    UInt16 count = 0;
    for(Int16 i = 0; i < 4; i++)
    {
        Int16 ny = (Int16) row + _directions[i][0];
        Int16 nx = (Int16) col + _directions[i][1];
        if(ny > 0 && ny <= _height && nx > 0 && nx <= _width)
        {
            if(gNkGameInstance.reactor[ny][nx].id.category == cat)
            {
                count++;
            }
        }
    }
    return count;
}

NkOrthoNeighborTiles nkReactorFindOrthoNeighborsOfCat(NkComponentCategory cat, UInt16 row, UInt16 col)
{
    _ensureReactorInitialized();
    return (NkOrthoNeighborTiles) {
        .north = row - 1 < 0 || gNkGameInstance.reactor[row - 1][col].id.category != cat
            ? null
            : &gNkGameInstance.reactor[row - 1][col],
        .south = row + 1 > _height || gNkGameInstance.reactor[row + 1][col].id.category != cat
            ? null
            : &gNkGameInstance.reactor[row + 1][col],
        .west = col - 1 < 0 || gNkGameInstance.reactor[row][col - 1].id.category != cat
            ? null
            : &gNkGameInstance.reactor[row][col - 1],
        .east = col + 1 > _width || gNkGameInstance.reactor[row][col + 1].id.category != cat
            ? null
            : &gNkGameInstance.reactor[row][col + 1]
    };
}

// soft
Void nkResetReactor()
{
    if(!gNkGameInstance.reactor)
    {
        return;
    }
    // not necessarily required to reset
    for(Int16 row = 0; row < _height; row++)
    {
        for(Int16 col = 0; col < _width; col++)
        {
            if(gNkGameInstance.reactor[row][col].id.category != NK_COMPONENT_INTERNAL)
            {
                nkTileToAir(&gNkGameInstance.reactor[row][col]);
            }
        }
    }
    gNkGameInstance.totalPower = 0.0f;
    gNkGameInstance.totalHeat = 0.0f;
}