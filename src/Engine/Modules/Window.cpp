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

static void CheckForOpeGLDebugContext()
{
    i32 openGLContextFlags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &openGLContextFlags);

    b32 isContextDebug = (openGLContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT);
    ExAssert(isContextDebug);
}

static void APIENTRY OpenGLDebugeCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void* userParam)
{
    ExUnused(length);
    ExUnused(userParam);

    ExLog("GLDEBUG(%u): %s\n", id, message);

    #define SourceCase(label) case label: {  } break
    switch (source)
    {
        SourceCase(GL_DEBUG_SOURCE_API);
        SourceCase(GL_DEBUG_SOURCE_WINDOW_SYSTEM);
        SourceCase(GL_DEBUG_SOURCE_SHADER_COMPILER);
        SourceCase(GL_DEBUG_SOURCE_THIRD_PARTY);
        SourceCase(GL_DEBUG_SOURCE_APPLICATION);
        SourceCase(GL_DEBUG_SOURCE_OTHER);

        default: { ExUnreachableCodePath(); } break;
    }
    #undef SourceCase

    #define TypeCase(label) case label: { ExLog("GLDEBUG(%u): Type: %s\n", id, #label); } break
    switch (type)
    {
        TypeCase(GL_DEBUG_TYPE_ERROR);
        TypeCase(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR);
        TypeCase(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR);
        TypeCase(GL_DEBUG_TYPE_PORTABILITY);
        TypeCase(GL_DEBUG_TYPE_PERFORMANCE);
        TypeCase(GL_DEBUG_TYPE_MARKER);
        TypeCase(GL_DEBUG_TYPE_PUSH_GROUP);
        TypeCase(GL_DEBUG_TYPE_POP_GROUP);
        TypeCase(GL_DEBUG_TYPE_OTHER);

        default: { ExUnreachableCodePath(); } break;
    }
    #undef TypeCase

    #define SeverityCase(label) case label: { ExLog("GLDEBUG(%u): Severity: %s\n", id, #label); } break;
    switch (severity)
    {
        SeverityCase(GL_DEBUG_SEVERITY_HIGH);
        SeverityCase(GL_DEBUG_SEVERITY_MEDIUM);
        SeverityCase(GL_DEBUG_SEVERITY_LOW);
        SeverityCase(GL_DEBUG_SEVERITY_NOTIFICATION);

        default: { ExUnreachableCodePath(); } break;
    }
    #undef SeverityCase

    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        ExCrash();
    }
}

static void SetupOpenGLDebugCallback()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugeCallback, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

static void GLFWFramebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    WindowModule* window = glfwGetWindowUserPointer(window);
    window.width = width;
    window.height = height;
    window.aspect = (f32)(width) / (f32)(height);

    glViewport(0, 0, width, height); // TODO: move this to the graphics module
}

void WindowModule::Init()
{
    #ifdef EX_DEBUG
    glfwSetErrorCallback(GLFWErrorCallback);
    #endif

    b32 isGLFWInitailized = glfwInit();
    ExAssertMsg(isGLFWInitailized, "Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef EX_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    #endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Ex Game", NULL, NULL);
    ExAssertMsg(window, "Failed to open window");

    glfwMakeContextCurrent(window);

    #pragma warning (push)
    #pragma warning (disable : 4191)
    b32 areGLFunctionsLoaded = gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));
    #pragma warning (pop)
    ExAssertMsg(areGLFunctionsLoaded, "Failed to load OpenGL functions");

    #ifdef EX_DEBUG
    CheckForOpeGLDebugContext();
    SetupOpenGLDebugCallback();
    #endif

    // NOTE: set various window callbacks
    glfwSetFramebufferSizeCallback(window, GLFWFramebufferSizeCallback);
}

void WindowModule::Fini()
{
    glfwTerminate();
}
