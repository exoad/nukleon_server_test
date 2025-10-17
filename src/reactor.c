#include "nukleon.h"
#include <stdlib.h>

NkGame gNkGameInstance = { 0 };

static NkUInt16 _height = -1;
static NkUInt16 _width = -1;

__nk_always_inline __nk_inline NkVoid _ensureReactorInitialized()
{
    if(!gNkGameInstance.reactor)
    {
        NK_PANIC("Game Instance's reactor is null!");
        return;
    }
}

__nk_always_inline __nk_hot __nk_inline NkVoid _checkReactorPos(NkUInt16 row, NkUInt16 col)
{
    if(row <= 0 || col <= 0 || row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
        return;
    }
}

NkUInt16 nkReactorGetWidth()
{
    return _width;
}

NkUInt16 nkReactorGetHeight()
{
    return _height;
}

NkVoid nkReactorSet(NkUInt16 row, NkUInt16 col, NkTile other)
{
    _ensureReactorInitialized();
    if(row > _height || col > _width)
    {
        NK_PANICF("The given row and column are out of bounds for %d, %d. Got: %d %d", _width, _height, row, col);
    }
    gNkGameInstance.reactor[row][col] = other;
}

NkBool nkIsValidLocInReactor(NkUInt16 row, NkUInt16 col)
{
    return col < nkReactorGetWidth() && row < nkReactorGetHeight();
}

NkVoid nkInitNkReactor(NkUInt16 width, NkUInt16 height)
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
    NkSize H = (NkSize) height;
    NkSize W = (NkSize) width;
    gNkGameInstance.reactor = (NkTile**) malloc(H * sizeof *(gNkGameInstance.reactor));
    if(!gNkGameInstance.reactor)
    {
        NK_PANICF("%s", "Failed to allocate reactor row pointer array.");
        return;
    }
    for(NkUInt16 y = 0; y < height; y++)
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
            for(NkUInt16 i = 0; i < y; i++)
            {
                free(gNkGameInstance.reactor[i]);
            }
            gNkGameInstance = (NkGame) { 0 };
            return;
        }
        for(NkUInt16 x = 0; x < width; x++)
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

NkVoid nkUninitNkReactor()
{
    if(!gNkGameInstance.reactor)
    {
        return;
    }
    for(NkUInt16 y = 0; y < _height; y++)
    {
        free(gNkGameInstance.reactor[y]);
    }
    gNkGameInstance = (NkGame) { 0 };
    _width = _height = 0;
}

NkUInt16 nkReactorGetFullNeighborsOfCat(NkComponentCategory cat, NkUInt16 row, NkUInt16 col)
{
    _ensureReactorInitialized();
    _checkReactorPos(row, col);
    NkUInt16 count = 0;
    for(NkInt16 dy = -1; dy <= 1; dy++)
    {
        for(NkInt16 dx = -1; dx <= 1; dx++)
        {
            if(dy == 0 && dx == 0)
            {
                continue;
            }
            NkInt16 ny = (NkInt16) row + dy;
            NkInt16 nx = (NkInt16) col + dx;
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

static const NkInt16 _directions[4][2] = {
    { -1, 0 }, // up
    { 1, 0 },  // down
    { 0, -1 }, // left
    { 0, 1 }   // right
};
NkUInt16 nkReactorGetOrthoNeighborsOf(NkComponentIdentifier id, NkUInt16 row, NkUInt16 col)
{
    _ensureReactorInitialized();
    _checkReactorPos(row, col);
    NkUInt16 count = 0;
    for(NkInt16 i = 0; i < 4; i++)
    {
        const NkInt16 ny = (NkInt16) row + _directions[i][0];
        const NkInt16 nx = (NkInt16) col + _directions[i][1];
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

NkUInt16 nkReactorGetOrthoNeighborsOfCat(NkComponentCategory cat, NkUInt16 row, NkUInt16 col)
{
    _ensureReactorInitialized();
    NkUInt16 count = 0;
    for(NkInt16 i = 0; i < 4; i++)
    {
        NkInt16 ny = (NkInt16) row + _directions[i][0];
        NkInt16 nx = (NkInt16) col + _directions[i][1];
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

NkOrthoNeighborTiles nkReactorFindOrthoNeighborsOfCat(NkComponentCategory cat, NkUInt16 row, NkUInt16 col)
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
NkVoid nkResetReactor()
{
    if(!gNkGameInstance.reactor)
    {
        return;
    }
    // not necessarily required to reset
    for(NkInt16 row = 0; row < _height; row++)
    {
        for(NkInt16 col = 0; col < _width; col++)
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