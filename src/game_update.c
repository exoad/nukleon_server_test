#include "nukleon_public.h"

NkVoid nkGameLoop()
{
    const NkFloat64 dt = 1.0 / NK_STATE_TPS_TARGET;
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
                    NK_PRINTLN("WARNING: Runaway catch up, stopping... (Behind by: %f)", time - nextTime);
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
                if(sleepTime > 0.5) // make sure we dont yield for when the sleep time is too small
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

NkVoid nkUpdate(NkFloat64 dt)
{
    
}