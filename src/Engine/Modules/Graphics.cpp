#include "Ex\Engine\Modules\Graphics.h"

#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning (pop)

#include "Ex/Systems/Logger.h"

static void CheckForOpeGLDebugContext()
{
    i32 openGLContextFlags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &openGLContextFlags);

    bool isContextDebug = (openGLContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT);
    ExAssert(isContextDebug);
}

static void APIENTRY OpenGLDebugeCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei, const char *message, const void*)
{
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

void GraphicsModule::Init(WindowModule* window)
{
    // Make the window's OpenGL context current on the calling thread
    glfwMakeContextCurrent((GLFWwindow*)(window->GetHandle()));

    #pragma warning (push)
    #pragma warning (disable : 4191)
    // Load OpenGL function pointers
    b32 areGLFunctionsLoaded = gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));
    #pragma warning (pop)

    if (!areGLFunctionsLoaded) ExLog("Failed to load OpenGL function pointers");
    ExAssert(areGLFunctionsLoaded);

    #ifdef EX_DEBUG
    CheckForOpeGLDebugContext();
    SetupOpenGLDebugCallback();
    #endif
}

void GraphicsModule::Fini()
{
    // TODO: to be implemented
}

void GraphicsModule::OnFramebufferResize(i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}
