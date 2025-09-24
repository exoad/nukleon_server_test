#include "nukleon_public.h"

NkVoid nkGameLoop()
{
    const NkFloat64 dt = 1.0 / NK_RULE_STATE_FPS_TARGET;
    NkUInt64 tickCount = 0;
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
                tickCount++;
                nextTime = startTime + tickCount * dt;
                if(++safety > 10000) // make sure it isnt catching up to an infinite bound
                {
                    NK_PRINTLN("WARNING: Runaway catch up, stopping... (behind by: %f)", time - nextTime);
                    tickCount = (NkUInt64) ((time - startTime) / dt) + 1;
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

#define REACTOR_WIDTH ((NkInt16) 20)
#define REACTOR_HEIGHT ((NkInt16) 10)

static NkInt32 _meltdownTicker = 0;

NkVoid nkUpdate(NkFloat64 dt)
{
    if(_meltdownTicker == NK_RULE_TOTAL_TICKS_FOR_MELTDOWN_WIPE + 1)
    {
        nkResetReactor();
    }
    NK_PRINTLN("%s", "====================================");
    NK_PRINTLN("DT = %5.6f", dt);
    NkFloat64 heatAdd = 0.0f;
    NkFloat64 powerAdd = 0.0f;
    NkBool willMeltdown = false;
    for(NkInt16 row = 0; row < REACTOR_HEIGHT; row++)
    {
        for(NkInt16 col = 0; col < REACTOR_WIDTH; col++)
        {
            const NkTile tile = gNkGameInstance.reactor[row][col];
            if(!tile.active)
            {
                continue; // dont auto flip non active cells for now
            }
            if(nkIsCellId(tile.id))
            {
                NkComponent* component = nkFindComponentById(tile.id);
                heatAdd += component->heatOutput;
                NK_PRINTLN("XX %5.4f with %5.4f (%s)", heatAdd, component->heatOutput, component->name);
                powerAdd += component->powerOutput;
            }
        }
    }
    gNkGameInstance.totalHeat += heatAdd;
    gNkGameInstance.totalPower += powerAdd;
    willMeltdown = gNkGameInstance.totalHeat > gNkGameInstance.maxHeat;
    if(willMeltdown)
    {
        _meltdownTicker++;
    }
    else
    {
        _meltdownTicker = 0;
    }

    // -- temporary rendering solution
    if(willMeltdown)
        NK_PRINTLN("Melting Down! (%5.4f)", gNkGameInstance.maxHeat);
    NK_PRINTLN("Total Heat = %5.4f", gNkGameInstance.totalHeat);
    NK_PRINTLN("Total Power = %5.4f", gNkGameInstance.totalPower);
    NK_PRINTLN("Meltdown Ticker: %d", _meltdownTicker);
    for(NkInt16 row = 0; row < REACTOR_HEIGHT; row++)
    {
        for(NkInt16 col = 0; col < REACTOR_WIDTH; col++)
        {
            const NkInt32 v = gNkGameInstance.reactor[row][col].id.id;
            if(v == NK_AIR)
            {
                NK_PRINT("%s", "[  ]");
            }
            else
            {
                NK_PRINT(gNkGameInstance.reactor[row][col].active ? "[%dO]" : "[%dY]", v);
            }
        }
        NK_PRINT("%s", "\n");
    }
}