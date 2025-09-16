#ifndef NUKLEON_PUBLIC_H
#define NUKLEON_PUBLIC_H

#include "nukleon_elements.h"

extern NkTile** reactor;

NkVoid newNkReactor(NkInt16 width, NkInt16 height);

NkVoid freeNkReactor();

#endif