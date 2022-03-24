#pragma once

#include "Ex\Core.h"
#include "Ex\App\InputEvent.h"

class Input
{
private:
    u32 nextEventIndex;
    u32 eventCount;
    InputEvent* events;
public:
    void Init(void* windowHandle);
    void Fini();

    u32 GetEventCount();
    InputEvent* GetEvents();
    void ClearEvents();
private:
    void PostEvent(InputEvent event);

    static void FramebufferSizeCallback(void* window, i32 width, i32 height);
    static void CloseCallback(void* window);
};
