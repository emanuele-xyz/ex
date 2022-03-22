#pragma once

#include "Ex/Core.h"

class WindowModuleEventsListener
{
public:
    virtual void OnFramebufferResize(i32 width, i32 height);
    virtual void OnCloseRequest();
};
