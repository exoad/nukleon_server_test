#include "nukleon.h"
#include <float.h>

static NkGameTick _gLastTick = { 0 };

NkGameTick* nkGetLastGameTick()
{
    return &_gLastTick;
}

static NkUInt64 _gTickIndex = 0;

NkUInt64 nkGetCurrentTickIndex()
{
    return _gTickIndex;
}

NkVoid nkGameLoop()
{
    const NkFloat64 dt = 1.0 / NK_RULE_STATE_FPS_TARGET;
    NkFloat64 startTime = nkTimeNowSec();
    NkFloat64 nextTime = startTime;
    NkFloat64 endTime = NK_STATE_RUN_SECONDS > 0 ? startTime + NK_STATE_RUN_SECONDS : -1.0;
    while(true)
    {
        NkFloat64 time = nkTimeNowSec();
        if(endTime > 0.0 && time >= endTime)
        {
            break;
        }
        if(time >= nextTime)
        {
            NkInt32 safety = 0;
            do // catching up! falling behind is scary D:
            {
                nkUpdate(dt);
#ifdef NK_ENABLE_UI_SAMPLER
                nkSample(dt);
#endif
                _gLastTick = (NkGameTick) {
                    .tickIndex = _gTickIndex,
                    .tickDurationMs = (NkUInt32) (dt * 1000.0),
                    .producedHeat = (NkFloat32) gNkGameInstance.totalHeat,
                    .producedPower = (NkFloat32) gNkGameInstance.totalPower,
                };
                _gTickIndex++;
                nextTime = startTime + _gTickIndex * dt;
                if(++safety > 10000) // make sure it isnt catching up to an infinite bound
                {
                    NK_PRINTLN("WARNING: Runaway catch up, stopping... (behind by: %f)", time - nextTime);
                    _gTickIndex = (NkUInt64) ((time - startTime) / dt) + 1;
                    nextTime = time + dt;
                    break;
                }
            } while(time >= nextTime);
        }
        else
        {
            NkFloat64 remaining = nextTime - time;
            if(remaining > 0)
            {
                NkFloat64 sleepTime = (remaining - 0.001) * 1000.0;
                if(sleepTime > 0.5) // make sure we dont sleep for when the time is too small
                {
                    nkSleepMs(sleepTime);
                }
                else
                {
                    nkYield();
                }
            }
        }
    }
}

NkVoid nkUpdate(__nk_unused NkFloat64 dt)
{
    static NkInt32 _meltdownTicker = 0;
    NkBool willMeltdown = false;
    if(_meltdownTicker == NK_RULE_TOTAL_TICKS_FOR_MELTDOWN_WIPE + 1)
    {
        nkResetReactor();
        _meltdownTicker = 0;
    }
    // the way to modify global mutable state seems very jank and very scary but this works
    // just recalculating everytime
    NkInt64 platingAddHeatCapacity = NK_RULE_GAME_REACTOR_STARTING_MAX_HEAT;
    NkFloat64 heatAdd = 0.0f;
    NkFloat64 powerAdd = 0.0f;
    for(NkInt16 row = 0; row < nkReactorGetHeight(); row++)
    {
        for(NkInt16 col = 0; col < nkReactorGetWidth(); col++)
        {
            NkTile* tile = &gNkGameInstance.reactor[row][col];
            if(!tile->active)
            {
                continue; // dont auto flip non active cells for now
            }
            if(nkIsCellId(tile->id))
            {
                NkComponent* component = nkFindComponentById(tile->id);
                heatAdd += component->heatOutput;
                powerAdd += component->powerOutput;
                tile->health -= 1;
                if(tile->health <= 0)
                {
                    tile->health = 0;
                    nkTileToAir(tile);
                }
            }
            else if(nkIsPlatingId(tile->id))
            {
                NkComponent* component = nkFindComponentById(tile->id);
                platingAddHeatCapacity += component->custom2;
            }
            else
            {
                // unknown component, do nothing
            }
        }
    }
    gNkGameInstance.totalHeat = nkClampFloat32((gNkGameInstance.totalHeat + heatAdd) - gNkGameInstance.naturalHeatRemoval, FLT_MAX, 0);
    gNkGameInstance.maxHeat = platingAddHeatCapacity;
    // clamp the produced heat
    if(gNkGameInstance.totalPower + powerAdd >= gNkGameInstance.maxPower)
    {
        gNkGameInstance.totalPower = gNkGameInstance.maxPower;
    }
    else
    {
        gNkGameInstance.totalPower += powerAdd;
    }
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
    for(NkInt16 row = 0; row < nkReactorGetHeight(); row++)
    {
        for(NkInt16 col = 0; col < nkReactorGetWidth(); col++)
        {
            const NkComponentIdentifier v = gNkGameInstance.reactor[row][col].id;
            NK_PRINT(gNkGameInstance.reactor[row][col].active ? "[%d_%dO]" : "[%dY]", v.category, v.id);
        }
        NK_PRINT("%s", "\n");
    }
#endif
}