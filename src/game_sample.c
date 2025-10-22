#include "nukleon.h"
#ifdef NK_ENABLE_UI_SAMPLER

static Tigr* _window = null;

NkInputEventState gNkCurrInputState[__INPUT_EVENT_POLL_STATES] = { 0 };
NkInputEventState gNkLastInputState[__INPUT_EVENT_POLL_STATES] = { 0 };

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
Int32 gNkSCurrMouseButtons = 0;
Int32 gNkSLastMouseButtons = 0;

// defined in physical pixels
#define CELL_SIZE 14
#define CELL_SPACING 2
#define TOTAL_INSET 100

Void nkPollInputs(Tigr* bmp)
{
    Int32 mouseX, mouseY, mouseButtons;
    tigrMouse(bmp, &mouseX, &mouseY, &mouseButtons);
    gNkSMouseLocation.x = mouseX;
    gNkSMouseLocation.y = mouseY;
    Bool mouseLeftDown = (mouseButtons & 1);
    Bool mouseMiddleDown = (mouseButtons & 2);
    Bool mouseRightDown = (mouseButtons & 4);
    for (int i = 0; i < 256; i++)
    {
        Bool isDown;
        Bool wasDown = gNkLastInputState[i];
        if(i == NK_S_LEFT_MOUSE_BUTTON)
        {
            isDown = mouseLeftDown;
        }
        else if(i == NK_S_MIDDLE_MOUSE_BUTTON)
        {
            isDown = mouseMiddleDown;
        }
        else if(i == NK_S_RIGHT_MOUSE_BUTTON)
        {
            isDown = mouseRightDown;
        }
        else
        {
            isDown = (Bool) tigrKeyDown(bmp, i);
        }
        // this is always latched but never unlatched as of this moment
        if(isDown && !wasDown)
        {
            gNkCurrInputState[i] = NK_S_INPUT_PRESSED;
        }
        else if(!isDown && wasDown)
        {
            gNkCurrInputState[i] = NK_S_INPUT_RELEASED;
        }
        gNkLastInputState[i] = isDown;
    }
}

Void nkConsumePolledInputs()
{
    for(int i = 0; i < 256; i++)
    {
        if(gNkCurrInputState[i] == NK_S_INPUT_PRESSED)
        {
            gNkCurrInputState[i] = NK_S_INPUT_DOWN;
        }
        else if(gNkCurrInputState[i] == NK_S_INPUT_RELEASED)
        {
            gNkCurrInputState[i] = NK_S_INPUT_UP;
        }
    }
}

__nk_hot Void nkSample(Float64 dt)
{
    static UInt64 frameCount = 0;
    static Float64 elapsedTime = 0.0;
    static Float32 fps = 0;
    nkPollInputs(_window);
    frameCount++;
    elapsedTime += dt;
    if(elapsedTime >= 1.0)
    {
        fps = frameCount;
        frameCount = 0;
        elapsedTime -= 1.0;
    }
    if(!tigrClosed(_window))
    {
        tigrClear(_window, COLOR_BLACK);
        // -- BEGIN
        Int32 _ignorebuttons;
        tigrMouse(_window, &gNkSMouseLocation.x, &gNkSMouseLocation.y, &_ignorebuttons);
        Bool showPopup = false;
        Int32 popupX, popupY;
        NkTile* popupTile = null;
        NkComponent* popupComponent = null;
        Int32 iy = TOTAL_INSET;
        if(NK_RULE_GAME_REACTOR_DISPLAY_VISUAL_HEAT_THRESHOLD >= (gNkGameInstance.maxHeat - gNkGameInstance.totalHeat) / gNkGameInstance.maxHeat)
        {
            tigrFillRect(_window, 0, 0, _window->w, _window->h, tigrRGB(0xFF * (gNkGameInstance.totalHeat / gNkGameInstance.maxHeat), 0, 0));
        }
        for(UInt16 row = 0; row < nkReactorGetHeight(); row++, iy += CELL_SIZE + CELL_SPACING)
        {
            Int32 ix = TOTAL_INSET;
            for(UInt16 col = 0; col < nkReactorGetWidth(); col++, ix += CELL_SIZE + CELL_SPACING)
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
                        // red x
                        tigrLine(_window, ix, iy, ix + CELL_SIZE, iy + CELL_SIZE, COLOR_RED);
                        tigrLine(_window, ix + CELL_SIZE, iy, ix, iy + CELL_SIZE, COLOR_RED);
                    }
                }
                else if(component->id.category == NK_COMPONENT_FUEL_CELL)
                {
                    tigrFillRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_YELLOW);
                }
                else if(component->id.category == NK_COMPONENT_PLATING)
                {
                    tigrFillRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_GRAY);
                }
                else if(component->id.category == NK_COMPONENT_VENTS)
                {
                    tigrFillRect(_window, ix, iy, CELL_SIZE, CELL_SIZE, COLOR_BLUE);
                }
                else
                {
                    tigrPrint(_window, tfont, ix + 2, iy + 2, COLOR_MAGENTA, "?");
                }
                tigrFillRect(_window, ix, iy, 4, (Int32) CELL_SIZE * (component->health <= 0 ? 0.f : (tile->health / component->health)), COLOR_GREEN);
                skip_render:
                    if(nkSIsMouseIn(&(NkRect2D) { { ix, iy }, { CELL_SIZE, CELL_SIZE } }))
                    {
                        showPopup = true;
                        popupX = ix + CELL_SIZE;
                        popupY = iy;
                        popupTile = tile;
                        popupComponent = component;
                    }
            }
        }
        if(showPopup && popupTile && popupComponent)
        {
#define POPUP_PADDING 2
            tigrFillRect(_window, popupX, popupY, 160, 90, COLOR_BLACK);
            tigrRect(_window, popupX, popupY, 160, 90, COLOR_WHITE);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + POPUP_PADDING, COLOR_RED, "%s", popupComponent->name);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 10 + POPUP_PADDING, COLOR_WHITE, "ID: %d,%d", popupTile->id.category, popupTile->id.id);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 20 + POPUP_PADDING, COLOR_WHITE, "Tier: %d", popupTile->tier);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 30 + POPUP_PADDING, COLOR_WHITE, "HP: %d (%d)", popupTile->health, popupComponent->health);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 40 + POPUP_PADDING, COLOR_WHITE, "Pwr: %5.2f", popupComponent->powerOutput);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 50 + POPUP_PADDING, COLOR_WHITE, "Heat: %5.2f", popupComponent->heatOutput);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 60 + POPUP_PADDING, COLOR_WHITE, "Last Tick H/P: %.2f/%.2f", popupTile->lastTickHeat, popupTile->lastTickPower);
            tigrPrint(_window, tfont, popupX + POPUP_PADDING, popupY + 70 + POPUP_PADDING, COLOR_WHITE, "Active? %s", nkCanonicalizeBool(popupTile->active));
        }
        // debug information
        tigrPrint(_window, tfont, 10, 10, COLOR_WHITE, "Delta Time: %5.4f", dt);
        tigrPrint(_window, tfont, 10, 20, COLOR_WHITE, "Tick Index: %llu", nkGetCurrentTickIndex());
        tigrPrint(_window, tfont, 10, 30, COLOR_RED, "Total Heat: %5.4f / %5.4f", gNkGameInstance.totalHeat, gNkGameInstance.maxHeat);
        tigrPrint(_window, tfont, 10, 40, COLOR_GREEN, "Total Power: %5.4f / %5.4f", gNkGameInstance.totalPower, gNkGameInstance.maxPower);
        tigrPrint(_window, tfont, 10, 50, COLOR_WHITE, "Reactor Size: %dx%d", nkReactorGetWidth(), nkReactorGetHeight());
        tigrPrint(_window, tfont, 10, 60, COLOR_WHITE, "Last Tick Took: %5.4fms", (Float64) nkGetLastGameTick()->tickDurationMs);
        tigrPrint(_window, tfont, 10, 70, COLOR_WHITE, "Last Tick Heat: %5.4f", nkGetLastGameTick()->producedHeat);
        tigrPrint(_window, tfont, 10, 80, COLOR_WHITE, "Last Tick Power: %5.4f", nkGetLastGameTick()->producedPower);
        tigrPrint(_window, tfont, 10, 90, COLOR_WHITE, "Meltdown Ticker: %d", nkGetLastGameTick()->meltdownTicker);
        tigrPrint(_window, tfont, 10, 100, COLOR_CYAN, "Self Dissipation Rate: %5.4f", gNkGameInstance.naturalHeatRemoval);
        tigrPrint(_window, tfont, 10, 110, COLOR_WHITE, "FPS: %.1f", fps);
        // -- END
        tigrUpdate(_window);
    }
}

Void nkInitSampler()
{
    _window = tigrWindow(800, 544, "nukleon_server_test", 0);
}

Void nkUninitSampler()
{
    if(_window != null)
    {
        tigrFree(_window);
    }
}

#else

Void nkPollInputs(__nk_unused Tigr* bmp)
{
}

Void nkConsumePolledInputs()
{
}

Void nkSample(__nk_unused Float64 dt)
{
}

Void nkInitSampler()
{
}

Void nkUninitSampler()
{
}

#endif