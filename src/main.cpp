#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#pragma warning (pop)

#include <stdio.h> // TODO: to be removed?
#include <stdlib.h> // TODO: to be removed?

#include "Ex/Core.h"

#define DEBUG_OUTPUT_SEPARATOR "---------------------------------------"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    ExUnused(window);

    glViewport(0, 0, width, height);
}

void GLAssertDebugContext()
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

    printf(
        DEBUG_OUTPUT_SEPARATOR "\n"
        "GLDEBUG(%u): %s"      "\n",
        id, message
    );

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

    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        ExCrash();
    }
}

void GLSetupDebugCallback()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugOutput, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}

void GLFWErrorCallback(int error, const char* description)
{
    printf(
        DEBUG_OUTPUT_SEPARATOR "\n"
        "GLFWDEBUG(%d): %s"    "\n",
        error, description
    );

    ExCrash();
}

void* ExAlloc(sz size)
{
    void* tmp = calloc(1, size);
    ExAssertMsg(tmp, "Failed to allocate memory");

    return tmp;
}

void ExFree(void* ptr)
{
    ExAssertMsg(ptr, "Freeing an null pointer");
    free(ptr);
}

void LoadFileData(const char* path, u8** data, u32* size)
{
    FILE* f = fopen(path, "rb");
    ExAssertFmt(f, "Failed to open file '%s'", path);

    fseek(f, 0, SEEK_END);
    *size = (u32)(ftell(f));
    fseek(f, 0, SEEK_SET);

    *data = (u8*)(ExAlloc(*size));
    fread(*data, *size, 1, f);
}

#define INFO_LOG_SIZE 1024

enum ShaderType
{
    SHADER_TYPE_VERT = GL_VERTEX_SHADER,
    SHADER_TYPE_FRAG = GL_FRAGMENT_SHADER,
};

u32 CreateShader(const c8* path, ShaderType type)
{
    u8* data = NULL;
    u32 size = 0;
    LoadFileData(path, &data, &size);

    u32 id = glCreateShader(type);
    i32 shaderSize = (i32)(size);
    glShaderSource(id, 1, (const c8**)(&data), &shaderSize);
    glCompileShader(id);

    b32 success = false;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        c8 log[INFO_LOG_SIZE] = {};
        glGetShaderInfoLog(id, INFO_LOG_SIZE, NULL, log);
        printf(
            DEBUG_OUTPUT_SEPARATOR                "\n"
            "SHADERDEBUG: failed to compile '%s'" "\n"
            "%s"                                  "\n",
            path, log
        );

        ExCrashMsg("Failed to compile shader");
    }

    return id;
}

u32 CreateProgram(const c8* vertPath, const c8* fragPath)
{
    u32 vertShader = CreateShader(vertPath, SHADER_TYPE_VERT);
    u32 fragShader = CreateShader(fragPath, SHADER_TYPE_FRAG);
    u32 id = glCreateProgram();

    glAttachShader(id, vertShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);

    b32 success = false;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        c8 log[INFO_LOG_SIZE] = {};
        glGetProgramInfoLog(id, INFO_LOG_SIZE, NULL, log);
        printf(
            DEBUG_OUTPUT_SEPARATOR                       "\n"
            "SHADERDEBUG: failed to link shader program" "\n"
            "Vertex   Shader: '%s'"                      "\n"
            "Fragment Shader: '%s'"                      "\n"
            "%s"                                         "\n",
            vertPath, fragPath, log
        );

        ExCrashMsg("Failed to create shader program");
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return id;
}

struct Vertex
{
    vec3 position;
};

struct MeshData
{
    Vertex* vertices;
    u32     vertexCount;
    u32     sizeofVertices;
    u32*    indices;
    u32     indexCount;
    u32     sizeofIndices;
};

void MeshDataInit(MeshData* /*data*/)
{
    // TODO: to be implemented
}

void MeshDataFini(MeshData* /*data*/)
{
    // TODO: to be implemented
}

void MeshDataInitQuad(MeshData* data)
{
    static Vertex vertices[] =
    {
        { +0.5f, +0.5f, +0.0f, },
        { +0.5f, -0.5f, +0.0f, },
        { -0.5f, -0.5f, +0.0f, },
        { -0.5f, +0.5f, +0.0f, },
    };

    static u32 indices[] =
    {
        0, 1, 2,
        2, 3, 0,
    };

    data->vertices = vertices;
    data->vertexCount = ExArraySize(vertices);
    data->sizeofVertices = sizeof(vertices);
    data->indices = indices;
    data->indexCount = ExArraySize(indices);
    data->sizeofIndices = sizeof(indices);
}

struct Mesh
{
    u32 vao;
    u32 vbo;
    u32 ebo;
};

void MeshInit(Mesh* mesh, MeshData* data)
{
    ExAssert(mesh->vao == 0);
    ExAssert(mesh->vbo == 0);
    ExAssert(mesh->ebo == 0);

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    {
        glGenBuffers(1, &mesh->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, data->sizeofVertices, data->vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &mesh->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->sizeofIndices, data->indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

void MeshFini(Mesh* mesh)
{
    ExAssert(mesh->vao != 0);
    ExAssert(mesh->vbo != 0);
    ExAssert(mesh->ebo != 0);

    glDeleteBuffers(1, &mesh->ebo);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);

    *mesh = {};
}

struct Material
{
    u32 program;
    // TODO: get uniforms here
};

void MaterialInit(Material* material, const c8* vertPath, const c8* fragPath)
{
    material->program = CreateProgram(vertPath, fragPath);
}

void MaterialFini(Material* /*material*/)
{
    // TODO: to be implemented
}

struct Context
{
    Mesh quadMesh;
    Material defaultMaterial;
};

void Setup(Context* ctx)
{
    // NOTE: initialize quad mesh
    {
        MeshData quadMeshData = {};
        MeshDataInitQuad(&quadMeshData);
        MeshInit(&ctx->quadMesh, &quadMeshData);
    }

    // NOTE: create default material
    {
        MaterialInit(&ctx->defaultMaterial, "data/shaders/shader.vert", "data/shaders/shader.frag");
    }
}

void MainLoop(Context* ctx)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // glUseProgram(ctx->program);
    // {
    //     glBindVertexArray(ctx->vao);
    //     {
    //         // glDrawArrays(GL_TRIANGLES, 0, (i32)(ctx->vertexCount));
    //         glDrawElements(GL_TRIANGLES, (i32)(ctx->indexCount), GL_UNSIGNED_INT, 0);
    //     }
    //     glBindVertexArray(0);
    // }
    // glUseProgram(0);
    MaterialBind(&ctx->defaultMaterial);
    {
        MeshDraw(&ctx->quadMesh);
    }
    MaterialUnbind();
}

int main()
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
    GLAssertDebugContext();
    GLSetupDebugCallback();
    #endif

    // NOTE: set various window callbacks
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    Context ctx = {};
    Setup(&ctx);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        MainLoop(&ctx);

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
