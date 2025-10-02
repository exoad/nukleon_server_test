#include "nukleon_public.h"

NkBool nkGPointInRect(const NkPoint2D* point, const NkRect2D* rect)
{
    if(!point || !rect)
    {
        return false;
    }
    return point->x >= rect->topLeft.x && point->x < (rect->topLeft.x + rect->size.width) && point->y >= rect->topLeft.y && point->y < (rect->topLeft.y + rect->size.height);
}