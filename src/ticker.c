#include "nukleon.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
static LARGE_INTEGER g_freq;

Void nkInitTime()
{
    QueryPerformanceFrequency(&g_freq);
}

Float64 nkTimeNowSec()
{
    LARGE_INTEGER c;
    QueryPerformanceCounter(&c);
    return (Float64) c.QuadPart / (Float64) g_freq.QuadPart;
}

Void nkSleepMs(Int32 ms)
{
    if(ms > 0)
    {
        Sleep((DWORD) ms);
    }
}

Void nkYield()
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
Void nkInitTime() {}

Float64 nkTimeNowSec()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (Float64) ts.tv_sec + (Float64) ts.tv_nsec / 1e9;
}

Void nkSleepMs(Int32 ms) {
    if(ms <= 0)
    {
        return;
    }
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

Void nkYield()
{
    sched_yield();
}
#endif
