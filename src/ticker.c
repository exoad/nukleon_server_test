#include "nukleon_public.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
static LARGE_INTEGER g_freq;

NkVoid nkInitTime()
{
    QueryPerformanceFrequency(&g_freq);
}

NkFloat64 nkTimeNowSec()
{
    LARGE_INTEGER c;
    QueryPerformanceCounter(&c);
    return (NkFloat64) c.QuadPart / (NkFloat64) g_freq.QuadPart;
}

NkVoid nkSleepMs(NkInt32 ms)
{
    if(ms > 0)
    {
        Sleep((DWORD) ms);
    }
}

NkVoid nkYield()
{
    if(!SwitchToThread())
    {
        Sleep(0);
    }
}
#else
#include <time.h>
#include <unistd.h>
#include <sched.h>
NkVoid nkInitTime() {}

NkFloat64 nkTimeNowSec()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (NkFloat64) ts.tv_sec + (NkFloat64) ts.tv_nsec / 1e9;
}

NkVoid nkSleepMs(NkInt32 ms) {
    if(ms <= 0)
    {
        return;
    }
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

NkVoid nkYield()
{
    sched_yield();
}
#endif
