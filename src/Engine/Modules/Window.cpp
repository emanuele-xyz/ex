#include "Ex/Engine/Modules/Window.h"

#include "Ex/Systems/Logger.h"

#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning (pop)

static void GLFWErrorCallback(int error, const char* description)
{
    ExLog("GLFWDEBUG(%d): %s\n", error, description);
    ExCrash(); // TODO: in release crash but in debug start debugging with debugbreak
}

static void GLFWFramebufferSizeCallback(GLFWwindow* handle, int width, int height)
{
    WindowModule* window = (WindowModule*)(glfwGetWindowUserPointer(handle));
    window->width = width;
    window->height = height;
    window->aspect = (f32)(width) / (f32)(height);

    ExAssert(window->resizeCallbackFn);
    window->resizeCallbackFn(width, height);
}

void WindowModule::Init(SettingsModule* settings)
{
    handle = nullptr;
    width = settings->windowWidth;
    height = settings->windowHeight;
    resizeCallbackFn = nullptr;

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

    handle = glfwCreateWindow(width, height, settings->appName, NULL, NULL);
    if (!handle) ExLog("Failed to crete GLFW window");
    ExAssert(handle);

    glfwSetWindowUserPointer((GLFWwindow*)(handle), this);

    glfwSetFramebufferSizeCallback((GLFWwindow*)(handle), GLFWFramebufferSizeCallback);
}

void WindowModule::Fini()
{
    glfwDestroyWindow((GLFWwindow*)(handle));
    glfwTerminate();
}
