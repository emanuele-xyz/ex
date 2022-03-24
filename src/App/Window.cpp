#include "Ex/App/Window.h"

#include "Ex/Systems/Logger.h"
#include "Ex/Systems/Memory.h"

#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning (pop)

void Window::Init(i32 width, i32 height, char* title)
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
}

void Window::Fini()
{
    glfwDestroyWindow((GLFWwindow*)(handle));
    glfwTerminate();
}

void* Window::GetHandle()
{
    return handle;
}

i32 Window::GetWidth()
{
    i32 w = 0;
    glfwGetWindowSize((GLFWwindow*)(handle), &w, nullptr);
    return w;
}

i32 Window::GetHeight()
{
    i32 h = 0;
    glfwGetWindowSize((GLFWwindow*)(handle), nullptr, &h);
    return h;
}

void Window::PollEvents()
{
    glfwPollEvents();
}

bool Window::ShouldCloseAndReset()
{
    bool tmp = (bool)(glfwWindowShouldClose((GLFWwindow*)(handle)));
    glfwSetWindowShouldClose((GLFWwindow*)(handle), false);
    return tmp;
}

bool Window::WasResizedAndReset()
{
    bool tmp = wasResized;
    wasResized = false;
    return tmp;
}

void Window::SwapBuffers()
{
    glfwSwapBuffers((GLFWwindow*)(handle));
}

void Window::GLFWErrorCallback(int error, const char* description)
{
    ExLog("GLFWDEBUG(%d): %s\n", error, description);
    ExCrash();
}

void Window::GLFWFramebufferSizeCallback(void* handle, int /*w*/, int /*h*/)
{
    Window* window = (Window*)(glfwGetWindowUserPointer((GLFWwindow*)(handle)));
    window->wasResized = true;
}
