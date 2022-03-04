#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#pragma warning (pop)

#include <stdio.h>

#include "Ex/Core.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    ExUnused(window);

    glViewport(0, 0, width, height);
}

void GLAssertDebugContext(void)
{
    i32 flags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    b32 isContextDebug = (flags & GL_CONTEXT_FLAG_DEBUG_BIT);
    ExAssertMsg(isContextDebug, "Failed to create OpenGL debug context");
}

void APIENTRY GLDebugOutput
(
    GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char *message,
    const void *userParam
)
{
    ExUnused(userParam);
    ExUnused(length);

    printf("GLDEBUG(%u): %s\n", id, message);

    #define SourceCase(label) case label: { puts("Source: " #label); } break
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

    #define TypeCase(label) case label: { puts("Type: " #label); } break
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

    #define SeverityCase(label) case label: { puts("Severity: " #label); } break;
    switch (severity)
    {
        SeverityCase(GL_DEBUG_SEVERITY_HIGH);
        SeverityCase(GL_DEBUG_SEVERITY_MEDIUM);
        SeverityCase(GL_DEBUG_SEVERITY_LOW);
        SeverityCase(GL_DEBUG_SEVERITY_NOTIFICATION);

        default: { ExUnreachableCodePath(); } break;
    }
    #undef SeverityCase

    ExCrash();
}

void GLSetupDebugCallback(void)
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugOutput, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

void GLFWErrorCallback(int error, const char* description)
{
    printf("GLFWDEBUG(%d): %s\n", error, description);

    ExCrash();
}

void MainLoop(void)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(void)
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

    b32 areGLFunctionsLoaded = gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress));
    ExAssertMsg(areGLFunctionsLoaded, "Failed to load OpenGL functions");

    #ifdef EX_DEBUG
    GLAssertDebugContext();
    GLSetupDebugCallback();
    #endif

    // NOTE: set various window callbacks
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        MainLoop();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
