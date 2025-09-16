#ifndef NUKLEON_PUBLIC_H
#define NUKLEON_PUBLIC_H

#include "nukleon_elements.h"

NkVoid update();

// --- NkReactor

extern NkTile** reactor;

NkInt16 nkReactorGetWidth();

NkInt16 nkReactorGetHeight();

NkVoid newNkReactor(NkInt16 width, NkInt16 height);

NkVoid freeNkReactor();

NkTile nkReactorGet(NkInt16 row, NkInt16 col);

NkVoid nkReactorSet(NkInt16 row, NkInt16 col, NkTile other);

#endif