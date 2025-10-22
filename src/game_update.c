#include "nukleon.h"
#include <float.h>

static NkGameTick _gLastTick = { 0 };

__nk_pure NkGameTick* nkGetLastGameTick()
{
    return &_gLastTick;
}

static UInt64 _gTickIndex = 0;

__nk_pure UInt64 nkGetCurrentTickIndex()
{
    return _gTickIndex;
}


__nk_hot Void nkGameLoop()
{
    NK_ASSERT(NK_RULE_UPDATE_TPS_TARGET > 0, "TPS must be greater than 0");
    NK_ASSERT(NK_RULE_SAMPLE_FPS_TARGET > 0, "FPS must be greater than 0");
    static const Float64 updateDT = 1.0 / NK_RULE_UPDATE_TPS_TARGET;
    static const Float64 sampleDT = 1.0 / NK_RULE_SAMPLE_FPS_TARGET;
    Float64 accumulator = 0.0;
    Float64 lastTime = nkTimeNowSec();
    Float64 endTime = NK_STATE_RUN_SECONDS > 0 ? lastTime + NK_STATE_RUN_SECONDS : -1.0;
    while(true)
    {
        Float64 currentTime = nkTimeNowSec();
        Float64 deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        if(endTime > 0.0 && currentTime >= endTime)
        {
            break;
        }
        accumulator += deltaTime;
        if (accumulator > updateDT * 10.0)
        {
            accumulator = updateDT * 10.0;
        }
        while (accumulator >= updateDT)
        {
            nkUpdate(updateDT);

            _gLastTick = (NkGameTick) {
                .tickIndex = _gTickIndex,
                .tickDurationMs = (UInt32) (updateDT * 1000.0),
                .producedHeat = (Float32) gNkGameInstance.totalHeat,
                .producedPower = (Float32) gNkGameInstance.totalPower,
            };
            _gTickIndex++;

            accumulator -= updateDT;
        }
#ifdef NK_ENABLE_UI_SAMPLER
        nkSample(deltaTime);
#endif
        Float64 targetNextFrameTime = currentTime + sampleDT;
        Float64 remainingTime = targetNextFrameTime - nkTimeNowSec();
        if (remainingTime > 0.0)
        {
            Float64 sleepTime = (remainingTime - 0.001) * 1000.0;
            if(sleepTime > 0.5)
            {
                nkSleepMs(sleepTime);
            }
            while (nkTimeNowSec() < targetNextFrameTime)
            {
                nkYield();
            }
        }
        else
        {
            nkYield();
        }
    }
}

// __nk_hot Void nkGameLoop()
// {
//     NK_ASSERT(NK_RULE_UPDATE_TPS_TARGET > 0, "TPS must be greater than 0");
//     NK_ASSERT(NK_RULE_SAMPLE_FPS_TARGET > 0, "FPS must be greater than 0");
//     NK_ASSERT(NK_RULE_UPDATE_TPS_TARGET <= NK_RULE_SAMPLE_FPS_TARGET, "TPS Must always be less than or equal to the FPS");
//     static const Float64 updateRatio = NK_RULE_SAMPLE_FPS_TARGET / NK_RULNK_RULE_UPDATE_TPS_TARGETE_UPDATE_TPS_TARGET;
//     static const Float64 sampleDT = 1.0 / NK_RULE_SAMPLE_FPS_TARGET;
//     static const Float64 updateDT = 1.0 / NK_RULE_UPDATE_TPS_TARGET;
//     Int16 fpsTickCounter = 0;
//     Float64 startTime = nkTimeNowSec();
//     Float64 accumulator = 0.0;
//     Float64 nextTime = startTime;
//     Float64 endTime = NK_STATE_RUN_SECONDS > 0 ? startTime + NK_STATE_RUN_SECONDS : -1.0;
//     while(true)
//     {
//         Float64 time = nkTimeNowSec();
//         if(endTime > 0.0 && time >= endTime)
//         {
//             break;
//         }
//         if(time >= nextTime)
//         {
//             Int32 safety = 0;
//             do // catching up! falling behind is scary D:
//             {
//                 if(fpsTickCounter >= updateRatio)
//                 {
//                     nkUpdate(updateDT);
//                     _gLastTick = (NkGameTick) {
//                         .tickIndex = _gTickIndex,
//                         .tickDurationMs = (UInt32) (updateDT * 1000.0),
//                         .producedHeat = (Float32) gNkGameInstance.totalHeat,
//                         .producedPower = (Float32) gNkGameInstance.totalPower,
//                     };
//                     _gTickIndex++;
//                     fpsTickCounter = 0;
//                 }
//                 fpsTickCounter++;
// #ifdef NK_ENABLE_UI_SAMPLER
//                 nkSample(sampleDT);
// #endif
//                 nextTime = startTime + (_gTickIndex * updateRatio + fpsTickCounter) * sampleDT;
//                 if(++safety > 1000) // lower catchup bound for graphics update
//                 {
//                     NK_PRINTLN("WARNING: Runaway (Sample) catch up, stopping... (behind by: %f)", time - nextTime);
//                     // _gTickIndex = (UInt64) ((time - startTime) / sampleDT) + 1;
//                     nextTime = time + sampleDT;
//                     _gTickIndex = (UInt64) ((nextTime - startTime) / updateRatio / sampleDT);
//                     fpsTickCounter = 1;
//                     break;
//                 }
//             } while(time >= nextTime);
//         }
//         else
//         {
//             Float64 remaining = nextTime - time;
//             if(remaining > 0)
//             {
//                 Float64 sleepTime = (remaining - 0.001) * 1000.0;
//                 if(sleepTime > 0.5) // make sure we dont sleep for when the time is too small
//                 {
//                     nkSleepMs(sleepTime);
//                 }
//                 else
//                 {
//                     nkYield();
//                 }
//             }
//         }
//     }
// }

__nk_hot __attribute__((optimize("-ffast-math"))) Void nkUpdate(__nk_unused Float64 sampleDT)
{
    static Int32 _meltdownTicker = 0;
    Bool willMeltdown = false;
    if(_meltdownTicker == NK_RULE_TOTAL_TICKS_FOR_MELTDOWN_WIPE + 1)
    {
        nkResetReactor();
        _meltdownTicker = 0;
    }
    // the way to modify global mutable state seems very jank and very scary but this works
    // just recalculating everytime
    Int64 platingAddHeatCapacity = NK_RULE_GAME_REACTOR_STARTING_MAX_HEAT;
    // Float64 heatAdd = 0.0f;
    Float64 powerAdd = 0.0f;
    // == inference pass ==
    // see how much power and heat each component in the reactor is going to produce this tick
    //
    // note: power is added globally while heat is computed and stored locally until later
    if(nkSIsButtonPressed(NK_S_LEFT_MOUSE_BUTTON))
    {
        NK_PRINTLN("Left Click @ %d,%d", gNkSMouseLocation.x, gNkSMouseLocation.y);
    }
    for(Int16 row = 0; row < nkReactorGetHeight(); row++)
    {
        for(Int16 col = 0; col < nkReactorGetWidth(); col++)
        {
            NkTile* tile = &gNkGameInstance.reactor[row][col];
            tile->tickHeat = 0.0f;
            if(!tile->active)
            {
                continue;
            }
            if(nkIsCellId(tile->id))
            {
                if(--(tile->health) <= 0)
                {
                    nkTileToAir(tile);
                    continue;
                }
                NkComponent* component = nkFindComponentById(tile->id);
                UInt16 adj = nkReactorGetOrthoNeighborsOfCat(NK_COMPONENT_FUEL_CELL, row, col);
                tile->tickHeat = component->heatOutput * ((adj * NK_RULE_GAME_FUEL_CELL_ADJACENCY_HEAT_BONUS) + 1);
                tile->lastTickPower = component->powerOutput * ((adj * NK_RULE_GAME_FUEL_CELL_ADJACENCY_POWER_BONUS) + 1);
                powerAdd += tile->lastTickPower;
            }
            else if(nkIsPlatingId(tile->id))
            {
                NkComponent* component = nkFindComponentById(tile->id);
                platingAddHeatCapacity += component->custom2;
            }
        }
    }
    // == distribution pass ==
    // move heat to adjacent containment tiles
    for(Int16 row = 0; row < nkReactorGetHeight(); row++)
    {
        for(Int16 col = 0; col < nkReactorGetWidth(); col++)
        {
            NkTile* tile = &gNkGameInstance.reactor[row][col];
            if(nkIsHotTile(tile) && nkIsCellId(tile->id))
            {
                NkOrthoNeighborTiles ventAdj = nkReactorFindOrthoNeighborsOfCat(NK_COMPONENT_VENTS, row, col);
                Int32 count = (ventAdj.east != null ? 1 : 0) + (ventAdj.north != null ? 1 : 0) + (ventAdj.south != null ? 1 : 0) + (ventAdj.west != null ? 1 : 0);
                if(count > 0)
                {
                    Float32 heatPerVent = tile->tickHeat / count;
                    if(ventAdj.east != null)
                    {
                        ventAdj.east->tickHeat = heatPerVent;
                    }
                    if(ventAdj.west != null)
                    {
                        ventAdj.west->tickHeat = heatPerVent;
                    }
                    if(ventAdj.north != null)
                    {
                        ventAdj.north->tickHeat = heatPerVent;
                    }
                    if(ventAdj.south != null)
                    {
                        ventAdj.south->tickHeat = heatPerVent;
                    }
                    tile->tickHeat = 0;
                }
            }
        }
    }
    // == calculation pass ==
    // apply the heat characteristics per cells and vent
    Float32 heatRemoved = 0.0f;
    Float32 heatGenerated = 0.0f;
    for(Int16 row = 0; row < nkReactorGetHeight(); row++)
    {
        for(Int16 col = 0; col < nkReactorGetWidth(); col++)
        {
            NkTile* tile = &gNkGameInstance.reactor[row][col];
            if (!tile->active)
            {
                continue;
            }
            if (nkIsCellId(tile->id))
            {
                heatGenerated += tile->tickHeat;
                tile->lastTickHeat = tile->tickHeat;
            }
            else if (nkIsVentId(tile->id))
            {
                heatRemoved += tile->tickHeat;
                tile->lastTickHeat = -tile->tickHeat;
            }
        }
    }
    gNkGameInstance.totalHeat = nkClampFloat32(gNkGameInstance.totalHeat + (heatGenerated - heatRemoved) - gNkGameInstance.naturalHeatRemoval, FLT_MAX, 0);
    gNkGameInstance.maxHeat = platingAddHeatCapacity;
    gNkGameInstance.totalPower = nkClampFloat32(gNkGameInstance.totalPower + powerAdd, gNkGameInstance.maxPower, 0);
    willMeltdown = gNkGameInstance.totalHeat > gNkGameInstance.maxHeat;
    _gLastTick.meltdownTicker = _meltdownTicker;
    if(willMeltdown)
    {
        _meltdownTicker++;
    }
    else
    {
        _meltdownTicker = 0;
    }

    // -- temporary rendering solution
#ifndef NK_ENABLE_UI_SAMPLER
    if(willMeltdown)
        NK_PRINTLN("Melting Down! (%5.4f)", gNkGameInstance.maxHeat);
    NK_PRINTLN("Total Heat = %5.4f (%5.4f%c)", gNkGameInstance.totalHeat, gNkGameInstance.totalHeat / gNkGameInstance.maxHeat, '%');
    NK_PRINTLN("Total Power = %5.4f (%5.4f%c)", gNkGameInstance.totalPower, gNkGameInstance.totalPower / gNkGameInstance.maxPower, '%');
    NK_PRINTLN("Meltdown Ticker: %d", _meltdownTicker);
    for(Int16 row = 0; row < nkReactorGetHeight(); row++)
    {
        for(Int16 col = 0; col < nkReactorGetWidth(); col++)
        {
            const NkComponentIdentifier v = gNkGameInstance.reactor[row][col].id;
            NK_PRINT(gNkGameInstance.reactor[row][col].active ? "[%d_%dO]" : "[%dY]", v.category, v.id);
        }
        NK_PRINT("%s", "\n");
    }
#endif
    nkConsumePolledInputs();
}