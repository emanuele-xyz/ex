#pragma once

#include "Ex/Core.h"

class Graphics
{
public:
    void Init(void* windowHandle);
    void Fini();

    void ClearColorBuffer(f32 r, f32 g, f32 b, f32 a);

    void ResizeFramebuffer(i32 width, i32 height);
};
