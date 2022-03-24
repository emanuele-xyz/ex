#pragma once

#include "Ex/Core.h"

class Window
{
private:
    void* handle;
    bool wasResized;

public:
    void Init(i32 width, i32 height, char* title);
    void Fini();

    void* GetHandle();
    i32 GetWidth();
    i32 GetHeight();

    void PollEvents();
    bool ShouldCloseAndReset();
    bool WasResizedAndReset();
    void SwapBuffers();

private:
    static void GLFWErrorCallback(int error, const char* description);
    static void GLFWFramebufferSizeCallback(void* handle, int width, int height);
};
