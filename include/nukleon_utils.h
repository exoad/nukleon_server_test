#ifndef NUKLEON_UTILS_H
#define NUKLEON_UTILS_H

#include "nukleon_shared.h"

// -- geometry
typedef struct NkPoint2D
{
    NkInt32 x;
    NkInt32 y;
} NkPoint2D;

typedef struct NkSize2D
{
    NkInt32 width;
    NkInt32 height;
} NkSize2D;

typedef struct NkRect2D
{
    NkPoint2D topLeft;
    NkSize2D size;
} NkRect2D;

NkBool nkGPointInRect(const NkPoint2D* point, const NkRect2D* rect);

NkFloat32 nkClampFloat32(NkFloat32 value, NkFloat32 max, NkFloat32 min);

#endif