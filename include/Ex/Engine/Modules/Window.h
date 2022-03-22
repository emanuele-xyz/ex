#pragma once

#include "Ex/Core.h"
#include "Ex/Engine/Modules/WindowEventsListener.h"

class WindowModule
{
private:
    void* handle;

    u32 listenersCount;
    WindowModuleEventsListener** listeners;

public:
    void Init(i32 width, i32 height, char* title);
    void Fini();

    void* GetHandle();

    void AddListener(WindowModuleEventsListener* listener);
    // TODO: should we add a remove listener function?
    // void RemoveListener(WindowModuleEventsListener* listener);

private:
    void EmitFramebufferResizeEvent(i32 width, i32 height);
    void EmitCloseRequestEvent();

private:
    static void GLFWErrorCallback(int error, const char* description);
    static void GLFWFramebufferSizeCallback(void* handle, int width, int height);
    static void GLFWCloseCallback(void* handle);
};
