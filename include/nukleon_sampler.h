#ifndef NUKLEON_SAMPLER_H
#define NUKLEON_SAMPLER_H

NkVoid nkSample(NkFloat64 dt);

/// @brief CALL AFTER INITIALIZING THE REACTOR
NkVoid nkInitSampler();

NkVoid nkUninitSampler();

#endif