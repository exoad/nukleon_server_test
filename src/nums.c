#include "nukleon.h"

__nk_always_inline __nk_pure __nk_hot inline NkFloat32 nkClampFloat32(NkFloat32 value, NkFloat32 max, NkFloat32 min)
{
    return value > max ? max : value < min ? min : value;
}