#include "nukleon.h"
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
#define COLOR_GRAY tigrRGB(0x80, 0x80, 0x80)

NkPoint2D gNkSMouseLocation = { 0, 0 };

// defined in physical pixels
#define CELL_SIZE 14
#define CELL_SPACING 2
#define TOTAL_INSET 100

__nk_hot NkVoid nkSample(__nk_unused NkFloat64 dt)
{
    if(!tigrClosed(_window))
    {
        tigrClear(_window, COLOR_BLACK);
        // -- BEGIN
        NkInt32 _ignorebuttons;
        tigrMouse(_window, &gNkSMouseLocation.x, &gNkSMouseLocation.y, &_ignorebuttons);
        NkBool showPopup = false;
        NkInt32 popupX, popupY;
        NkTile* popupTile = null;
        NkComponent* popupComponent = null;
        NkInt32 iy = TOTAL_INSET;
        if(NK_RULE_GAME_REACTOR_DISPLAY_VISUAL_HEAT_THRESHOLD >= (gNkGameInstance.maxHeat - gNkGameInstance.totalHeat) / gNkGameInstance.maxHeat)
        {
            tigrFillRect(_window, 0, 0, _window->w, _window->h, tigrRGB(0xFF * (gNkGameInstance.totalHeat / gNkGameInstance.maxHeat), 0, 0));
        }
        for(NkUInt16 row = 0; row < nkReactorGetHeight(); row++, iy += CELL_SIZE + CELL_SPACING)
        {
            NkInt32 ix = TOTAL_INSET;
            for(NkUInt16 col = 0; col < nkReactorGetWidth(); col++, ix += CELL_SIZE + CELL_SPACING)
            {
                NkTile* tile = &gNkGameInstance.reactor[row][col];
                NkComponent* component = nkFindComponentById(tile->id);
                if(!tile->active)
                {
                    tigrFillRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_GRAY);
                    goto skip_render;
                }
                if(component->id.category == NK_COMPONENT_INTERNAL)
                {
                    if(component->id.id == NK_AIR)
                    {
                        tigrRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_WHITE);
                    }
                    else if(component->id.id == NK_BARRIER)
                    {
                        tigrLine(_window, ix, iy, ix + CELL_SIZE, iy + CELL_SIZE, COLOR_RED);
                        tigrLine(_window, ix + CELL_SIZE, iy, ix, iy + CELL_SIZE, COLOR_RED);
                    }
                }
                else if(component->id.category == NK_COMPONENT_SINGLE_FUEL_CELL)
                {
                    tigrFillRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_YELLOW);
                }
                else if(component->id.category == NK_COMPONENT_PLATING)
                {
                    tigrFillRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_GREEN);
                }
                else
                {
                    tigrPrint(_window, tfont, ix + 2, iy + 2, COLOR_MAGENTA, "?");
                }
                skip_render:
                if(nkSIsMouseIn(&(NkRect2D) { { ix, iy }, { CELL_SIZE, CELL_SIZE } }))
                {
                    showPopup = true;
                    popupX = ix + CELL_SIZE;
                    popupY = iy;
                    popupTile = tile;
                    popupComponent = component;
                }
                // tigrFillRect(_window, ix, iy, 4, (NkInt32) CELL_SIZE * (component->health <= 0 ? 0.f : (tile->health / component->health)), COLOR_GREEN);
            }
        }
        if(showPopup && popupTile && popupComponent)
        {
#define POPUP_PADDING 2
            tigrFillRect(_window, popupX, popupY, 120, 70, COLOR_BLACK);
            tigrRect(_window, popupX, popupY, 120, 70, COLOR_WHITE);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + POPUP_PADDING, COLOR_RED, "%s", popupComponent->name);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 10 + POPUP_PADDING, COLOR_WHITE, "ID: %d,%d", popupTile->id.category, popupTile->id.id);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 20 + POPUP_PADDING, COLOR_WHITE, "Tier: %d", popupTile->tier);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 30 + POPUP_PADDING, COLOR_WHITE, "HP: %d (%d)", popupTile->health, popupComponent->health);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 40 + POPUP_PADDING, COLOR_WHITE, "Pwr: %5.2f", popupComponent->powerOutput);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 50 + POPUP_PADDING, COLOR_WHITE, "Heat: %5.2f", popupComponent->heatOutput);
        }
        // debug information
        tigrPrint(_window, tfont, 10, 10, COLOR_WHITE, "Delta Time: %5.4f", dt);
        tigrPrint(_window, tfont, 10, 20, COLOR_WHITE, "Tick Index: %llu", nkGetCurrentTickIndex());
        tigrPrint(_window, tfont, 10, 30, COLOR_RED, "Total Heat: %5.4f / %5.4f", gNkGameInstance.totalHeat, gNkGameInstance.maxHeat);
        tigrPrint(_window, tfont, 10, 40, COLOR_GREEN, "Total Power: %5.4f / %5.4f", gNkGameInstance.totalPower, gNkGameInstance.maxPower);
        tigrPrint(_window, tfont, 10, 50, COLOR_WHITE, "Reactor Size: %dx%d", nkReactorGetWidth(), nkReactorGetHeight());
        tigrPrint(_window, tfont, 10, 60, COLOR_WHITE, "Last Tick Took: %5.4fms", (NkFloat64) nkGetLastGameTick()->tickDurationMs);
        tigrPrint(_window, tfont, 10, 70, COLOR_WHITE, "Last Tick Heat: %5.4f", nkGetLastGameTick()->producedHeat);
        tigrPrint(_window, tfont, 10, 80, COLOR_WHITE, "Last Tick Power: %5.4f", nkGetLastGameTick()->producedPower);
        tigrPrint(_window, tfont, 10, 90, COLOR_WHITE, "Meltdown Ticker: %d", nkGetLastGameTick()->meltdownTicker);
        tigrPrint(_window, tfont, 10, 100, COLOR_CYAN, "Self Dissipation Rate: %5.4f", gNkGameInstance.naturalHeatRemoval);
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