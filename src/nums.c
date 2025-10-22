#include "nukleon.h"

__nk_always_inline __nk_pure __nk_hot inline Float32 nkClampFloat32(Float32 value, Float32 max, Float32 min)
{
    return value > max ? max : value < min ? min : value;
}