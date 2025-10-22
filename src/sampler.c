#include "nukleon.h"

Bool nkSIsMouseIn(NkRect2D* rect)
{
    return nkGPointInRect(&gNkSMouseLocation, rect);
}

Bool nkSIsButtonPressed(Int32 keyCode)
{
    return gNkCurrInputState[keyCode] == NK_S_INPUT_PRESSED;
}

Bool nkSIsButtonDown(Int32 keyCode)
{
    return gNkCurrInputState[keyCode] == NK_S_INPUT_PRESSED || gNkCurrInputState[keyCode] == NK_S_INPUT_DOWN;
}

Bool nkSIsButtonReleased(Int32 keyCode)
{
    return gNkCurrInputState[keyCode] == NK_S_INPUT_RELEASED;
}