#include "nukleon_utils.h"

NkFloat32 nkClampFloat32(NkFloat32 value, NkFloat32 max, NkFloat32 min)
{
    return value > max ? max : max < min ? min : value;
}