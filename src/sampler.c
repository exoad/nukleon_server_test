#include "nukleon.h"

NkBool nkSIsMouseIn(NkRect2D* rect)
{
    return nkGPointInRect(&gNkSMouseLocation, rect);
}