#include "nukleon_sampler.h"

NkBool nkSIsMouseIn(NkRect2D* rect)
{
    return nkGPointInRect(&gNkSMouseLocation, rect);
}