#ifndef NUKLEON_SAMPLER_H
#define NUKLEON_SAMPLER_H

#include "nukleon_types.h"
#include "nukleon_utils.h"

NkVoid nkSample(NkFloat64 dt);

/// @brief CALL AFTER INITIALIZING THE REACTOR
NkVoid nkInitSampler();

NkVoid nkUninitSampler();

extern NkPoint2D gNkSMouseLocation;

NkBool nkSIsMouseIn(NkRect2D* rect);

#endif