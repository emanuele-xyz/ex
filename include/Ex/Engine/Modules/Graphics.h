#pragma once

#include "Ex/Core.h"
#include "Ex/Engine/Modules/WindowEventsListener.h"

class GraphicsModule : public WindowModuleEventsListener
{
public:
    void Init(void* windowHandle);
    void Fini();

    void OnFramebufferResize(i32 width, i32 height);
};
