#ifndef NUKLEON_SAMPLER_H
#define NUKLEON_SAMPLER_H

#include "nukleon_types.h"
#include "nukleon_utils.h"

#include "../tigr.h"

Void nkSample(Float64 dt);

/// @brief CALL AFTER INITIALIZING THE REACTOR
Void nkInitSampler();

Void nkUninitSampler();

extern NkPoint2D gNkSMouseLocation;

#define __INPUT_EVENT_POLL_STATES 256

#define NK_S_LEFT_MOUSE_BUTTON (__INPUT_EVENT_POLL_STATES - 3)
#define NK_S_MIDDLE_MOUSE_BUTTON (__INPUT_EVENT_POLL_STATES - 2)
#define NK_S_RIGHT_MOUSE_BUTTON (__INPUT_EVENT_POLL_STATES - 1)

typedef enum
{
    NK_S_INPUT_UP = 0,
    NK_S_INPUT_PRESSED = 1,
    NK_S_INPUT_DOWN = 2,
    NK_S_INPUT_RELEASED = 3
} NkInputEventState;

extern NkInputEventState gNkCurrInputState[__INPUT_EVENT_POLL_STATES];
extern NkInputEventState gNkLastInputState[__INPUT_EVENT_POLL_STATES];

Void nkPollInputs(Tigr* bmp);

Void nkConsumePolledInputs();

Bool nkSIsButtonPressed(Int32 keyCode);

Bool nkSIsButtonDown(Int32 keyCode);

Bool nkSIsButtonReleased(Int32 keyCode);

Bool nkSIsMouseIn(NkRect2D* rect);

#endif