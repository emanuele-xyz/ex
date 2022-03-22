#include "Ex/Engine/Modules/Window.h"

#include "Ex/Systems/Logger.h"
#include "Ex/Systems/Memory.h"

#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning (pop)

void WindowModule::Init(i32 width, i32 height, char* title)
{
    #ifdef EX_DEBUG
    glfwSetErrorCallback(GLFWErrorCallback);
    #endif

    b32 isGLFWInitailized = glfwInit();
    if (!isGLFWInitailized) ExLog("Failed to initialize GLFW");
    ExAssert(isGLFWInitailized);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef EX_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    #endif

    handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!handle) ExLog("Failed to crete GLFW window");
    ExAssert(handle);

    glfwSetWindowUserPointer((GLFWwindow*)(handle), this);

    glfwSetFramebufferSizeCallback((GLFWwindow*)(handle), (GLFWframebuffersizefun)(GLFWFramebufferSizeCallback));

    // TODO: use a Vec
    listenersCount = 16;
    listeners = ExArrayAlloc<WindowModuleEventsListener*>(listenersCount);
}

void WindowModule::Fini()
{
    ExFree(listeners);
    listenersCount = 0;

    glfwDestroyWindow((GLFWwindow*)(handle));
    glfwTerminate();
}

void* WindowModule::GetHandle()
{
    return handle;
}

void WindowModule::AddListener(WindowModuleEventsListener* listener)
{
    for (u32 i = 0; i < listenersCount; i++)
    {
        if (listeners[i] == nullptr)
        {
            listeners[i] = listener;
        }
    }
}

void WindowModule::EmitFramebufferResizeEvent(i32 width, i32 height)
{
    for (u32 i = 0; i < listenersCount; i++)
    {
        if (listeners[i] != nullptr)
        {
            listeners[i]->OnFramebufferResize(width, height);
        }
    }
}

void WindowModule::EmitCloseRequestEvent()
{
    for (u32 i = 0; i < listenersCount; i++)
    {
        if (listeners[i] != nullptr)
        {
            listeners[i]->OnCloseRequest();
        }
    }
}

void WindowModule::GLFWErrorCallback(int error, const char* description)
{
    ExLog("GLFWDEBUG(%d): %s\n", error, description);
    ExCrash();
}

void WindowModule::GLFWFramebufferSizeCallback(void* handle, int width, int height)
{
    WindowModule* window = (WindowModule*)(glfwGetWindowUserPointer((GLFWwindow*)(handle)));
    window->EmitFramebufferResizeEvent(width, height);
}

void WindowModule::GLFWCloseCallback(void* handle)
{
    WindowModule* window = (WindowModule*)(glfwGetWindowUserPointer((GLFWwindow*)(handle)));
    window->EmitCloseRequestEvent();
}

