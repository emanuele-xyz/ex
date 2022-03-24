#pragma once

#include "Ex\Core.h"

enum class InputEventType
{
    None,
    FramebufferSize,
    Close,
};

struct FramebufferSizeInputEvent
{
    i32 width;
    i32 height;
};

struct InputEvent
{
    InputEventType type;
    union
    {
        FramebufferSizeInputEvent framebufferSize;
    };
};
