#pragma once

#include "Ex/Core.h"
#include "Ex/Engine/Modules/Window.h"

class GraphicsModule
{
public:
    void Init(WindowModule* window);
    void Fini();

    void OnFramebufferResize(i32 width, i32 height);
};
