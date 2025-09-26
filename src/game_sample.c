#include "nukleon_public.h"
#ifdef NK_ENABLE_UI_SAMPLER
#include "../tigr.h"

static Tigr* _window = null;

#define COLOR_BLACK tigrRGB(0x0, 0x0, 0x0)
#define COLOR_WHITE tigrRGB(0xFF, 0xFF, 0xFF)
#define COLOR_RED tigrRGB(0xFF, 0x0, 0x0)
#define COLOR_GREEN tigrRGB(0x0, 0xFF, 0x0)
#define COLOR_BLUE tigrRGB(0x0, 0x0, 0xFF)
#define COLOR_YELLOW tigrRGB(0xFF, 0xFF, 0x0)
#define COLOR_CYAN tigrRGB(0x0, 0xFF, 0xFF)
#define COLOR_MAGENTA tigrRGB(0xFF, 0x0, 0xFF)

static inline NkVoid _pushChanges()
{
    tigrUpdate(_window);
}

// defined in physical pixels
#define CELL_SIZE 14
#define CELL_SPACING 2
#define TOTAL_INSET 100

NkVoid nkSample(NkFloat64 dt)
{
    (NkVoid) dt;
    if(!tigrClosed(_window))
    {
        tigrClear(_window, COLOR_BLACK);
        // -- BEGIN
        tigrPrint(_window, tfont, 10, 10, tigrRGB(0xFF, 0xFF, 0xFF), "Delta Time: %5.4f", dt);
        tigrPrint(_window, tfont, 10, 20, tigrRGB(0xFF, 0xFF, 0xFF), "Tick Index: %llu", nkGetCurrentTickIndex());
        tigrPrint(_window, tfont, 10, 30, COLOR_RED, "Total Heat: %5.4f / %5.4f", gNkGameInstance.totalHeat, gNkGameInstance.maxHeat);
        tigrPrint(_window, tfont, 10, 40, COLOR_GREEN, "Total Power: %5.4f / %5.4f", gNkGameInstance.totalPower, gNkGameInstance.maxPower);
        NkInt32 iy = TOTAL_INSET;
        for(NkUInt16 row = 0; row < nkReactorGetHeight(); row++, iy += CELL_SIZE + CELL_SPACING)
        {
            NkInt32 ix = TOTAL_INSET;
            for(NkUInt16 col = 0; col < nkReactorGetWidth(); col++, ix += CELL_SIZE + CELL_SPACING)
            {
                const NkTile* tile = &gNkGameInstance.reactor[row][col];
                if(tile->id.id == NK_AIR)
                {
                    tigrRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_WHITE);
                }
                else if(tile->id.id == NK_BARRIER)
                {
                    tigrLine(_window, ix, iy, ix + CELL_SIZE, iy + CELL_SIZE, COLOR_RED);
                    tigrLine(_window, ix + CELL_SIZE, iy, ix, iy + CELL_SIZE, COLOR_RED);
                }
                else if(tile->id.category == NK_COMPONENT_SINGLE_FUEL_CELL)
                {
                    tigrFillRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_YELLOW);
                }
                else
                {
                    tigrPrint(_window, tfont, ix + 2, iy + 2, COLOR_MAGENTA, "?");
                }
            }
        }
        // -- END
        tigrUpdate(_window);
    }
}

NkVoid nkInitSampler()
{
    _window = tigrWindow(800, 544, "nukleon_server_test", TIGR_AUTO);
}

NkVoid nkUninitSampler()
{
    if(_window != null)
    {
        tigrFree(_window);
    }
}

#else

NkVoid nkSample(NkFloat64 dt)
{
    (NkVoid) dt;
}

NkVoid nkInitSampler()
{
}

NkVoid nkUninitSampler()
{
}

#endif