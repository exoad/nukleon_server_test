#ifndef NUKLEON_UTILS_H
#define NUKLEON_UTILS_H

// -- geometry
typedef struct
{
    NkInt32 x;
    NkInt32 y;
} NkPoint2D;

typedef struct
{
    NkInt32 width;
    NkInt32 height;
} NkSize2D;

typedef struct
{
    NkPoint2D topLeft;
    NkSize2D size;
} NkRect2D;

NkBool nkGPointInRect(const NkPoint2D* point, const NkRect2D* rect);

#endif