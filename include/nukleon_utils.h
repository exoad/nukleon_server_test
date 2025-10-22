#ifndef NUKLEON_UTILS_H
#define NUKLEON_UTILS_H

#include "nukleon.h"

// -- geometry
typedef struct NkPoint2D
{
    Int32 x;
    Int32 y;
} NkPoint2D;

typedef struct Size2D
{
    Int32 width;
    Int32 height;
} Size2D;

typedef struct NkRect2D
{
    NkPoint2D topLeft;
    Size2D size;
} NkRect2D;

Bool nkGPointInRect(const NkPoint2D* point, const NkRect2D* rect);

Float32 nkClampFloat32(Float32 value, Float32 max, Float32 min);

#endif