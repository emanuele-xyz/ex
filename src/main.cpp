#pragma warning (push, 0)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stb_image/stb_image.h>
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
    vec2 textureCoordinates;
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
        { { +0.5f, +0.5f, +0.0f, }, { 1.0f, 1.0f }, },
        { { +0.5f, -0.5f, +0.0f, }, { 1.0f, 0.0f }, },
        { { -0.5f, -0.5f, +0.0f, }, { 0.0f, 0.0f }, },
        { { -0.5f, +0.5f, +0.0f, }, { 0.0f, 1.0f }, },
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
    u32 vertexCount;
    u32 indexCount;
};

void MeshInit(Mesh* mesh, MeshData* data)
{
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    {
        glGenBuffers(1, &mesh->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, data->sizeofVertices, data->vertices, GL_STATIC_DRAW);
        // NOTE: position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
        glEnableVertexAttribArray(0);
        // NOTE: texture coordinates attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, textureCoordinates)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &mesh->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->sizeofIndices, data->indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);

    mesh->vertexCount = data->vertexCount;
    mesh->indexCount = data->indexCount;
}

void MeshFini(Mesh* mesh)
{
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);
}

enum TextureWrapping
{
    TEXTURE_WRAP_REPEAT          = GL_REPEAT,
    TEXTURE_WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
    TEXTURE_WRAP_CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
    TEXTURE_WRAP_CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
};

enum TextureMinFiltering
{
    TEXTURE_MIN_FILTER_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    TEXTURE_MIN_FILTER_LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
    TEXTURE_MIN_FILTER_NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
    TEXTURE_MIN_FILTER_LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
};

enum TextureMagFiltering
{
    TEXTURE_MAG_FILTER_NEAREST = GL_NEAREST,
    TEXTURE_MAG_FILTER_LINEAR  = GL_LINEAR,
};

enum TextureUnitId
{
    TEXTURE_UNIT_ID_NONE = -1,
    TEXTURE_UNIT_ID_0,
    TEXTURE_UNIT_ID_1,
    TEXTURE_UNIT_ID_2,
    TEXTURE_UNIT_ID_3,
    TEXTURE_UNIT_ID_4,
    TEXTURE_UNIT_ID_5,
    TEXTURE_UNIT_ID_6,
    TEXTURE_UNIT_ID_7,
    TEXTURE_UNIT_ID_8,
    TEXTURE_UNIT_ID_9,
    TEXTURE_UNIT_ID_10,
    TEXTURE_UNIT_ID_11,
    TEXTURE_UNIT_ID_12,
    TEXTURE_UNIT_ID_13,
    TEXTURE_UNIT_ID_14,
    TEXTURE_UNIT_ID_15,
    TEXTURE_UNIT_ID_16,
    TEXTURE_UNIT_ID_17,
    TEXTURE_UNIT_ID_18,
    TEXTURE_UNIT_ID_19,
    TEXTURE_UNIT_ID_20,
    TEXTURE_UNIT_ID_21,
    TEXTURE_UNIT_ID_22,
    TEXTURE_UNIT_ID_23,
    TEXTURE_UNIT_ID_24,
    TEXTURE_UNIT_ID_25,
    TEXTURE_UNIT_ID_26,
    TEXTURE_UNIT_ID_27,
    TEXTURE_UNIT_ID_28,
    TEXTURE_UNIT_ID_29,
    TEXTURE_UNIT_ID_30,
    TEXTURE_UNIT_ID_31,
};

enum TextureUnitCode
{
    TEXTURE_UNIT_CODE_NONE = 0,
    TEXTURE_UNIT_CODE_0    = GL_TEXTURE0,
    TEXTURE_UNIT_CODE_1    = GL_TEXTURE1,
    TEXTURE_UNIT_CODE_2    = GL_TEXTURE2,
    TEXTURE_UNIT_CODE_3    = GL_TEXTURE3,
    TEXTURE_UNIT_CODE_4    = GL_TEXTURE4,
    TEXTURE_UNIT_CODE_5    = GL_TEXTURE5,
    TEXTURE_UNIT_CODE_6    = GL_TEXTURE6,
    TEXTURE_UNIT_CODE_7    = GL_TEXTURE7,
    TEXTURE_UNIT_CODE_8    = GL_TEXTURE8,
    TEXTURE_UNIT_CODE_9    = GL_TEXTURE9,
    TEXTURE_UNIT_CODE_10   = GL_TEXTURE10,
    TEXTURE_UNIT_CODE_11   = GL_TEXTURE11,
    TEXTURE_UNIT_CODE_12   = GL_TEXTURE12,
    TEXTURE_UNIT_CODE_13   = GL_TEXTURE13,
    TEXTURE_UNIT_CODE_14   = GL_TEXTURE14,
    TEXTURE_UNIT_CODE_15   = GL_TEXTURE15,
    TEXTURE_UNIT_CODE_16   = GL_TEXTURE16,
    TEXTURE_UNIT_CODE_17   = GL_TEXTURE17,
    TEXTURE_UNIT_CODE_18   = GL_TEXTURE18,
    TEXTURE_UNIT_CODE_19   = GL_TEXTURE19,
    TEXTURE_UNIT_CODE_20   = GL_TEXTURE20,
    TEXTURE_UNIT_CODE_21   = GL_TEXTURE21,
    TEXTURE_UNIT_CODE_22   = GL_TEXTURE22,
    TEXTURE_UNIT_CODE_23   = GL_TEXTURE23,
    TEXTURE_UNIT_CODE_24   = GL_TEXTURE24,
    TEXTURE_UNIT_CODE_25   = GL_TEXTURE25,
    TEXTURE_UNIT_CODE_26   = GL_TEXTURE26,
    TEXTURE_UNIT_CODE_27   = GL_TEXTURE27,
    TEXTURE_UNIT_CODE_28   = GL_TEXTURE28,
    TEXTURE_UNIT_CODE_29   = GL_TEXTURE29,
    TEXTURE_UNIT_CODE_30   = GL_TEXTURE30,
    TEXTURE_UNIT_CODE_31   = GL_TEXTURE31,
};

struct TextureUnit
{
    TextureUnitId   id;
    TextureUnitCode code;
};

constexpr TextureUnit TEXTURE_UNIT_NONE = { TEXTURE_UNIT_ID_NONE, TEXTURE_UNIT_CODE_NONE };
constexpr TextureUnit TEXTURE_UNIT_0    = { TEXTURE_UNIT_ID_0,    TEXTURE_UNIT_CODE_0    };
constexpr TextureUnit TEXTURE_UNIT_1    = { TEXTURE_UNIT_ID_1,    TEXTURE_UNIT_CODE_1    };
constexpr TextureUnit TEXTURE_UNIT_2    = { TEXTURE_UNIT_ID_2,    TEXTURE_UNIT_CODE_2    };
constexpr TextureUnit TEXTURE_UNIT_3    = { TEXTURE_UNIT_ID_3,    TEXTURE_UNIT_CODE_3    };
constexpr TextureUnit TEXTURE_UNIT_4    = { TEXTURE_UNIT_ID_4,    TEXTURE_UNIT_CODE_4    };
constexpr TextureUnit TEXTURE_UNIT_5    = { TEXTURE_UNIT_ID_5,    TEXTURE_UNIT_CODE_5    };
constexpr TextureUnit TEXTURE_UNIT_6    = { TEXTURE_UNIT_ID_6,    TEXTURE_UNIT_CODE_6    };
constexpr TextureUnit TEXTURE_UNIT_7    = { TEXTURE_UNIT_ID_7,    TEXTURE_UNIT_CODE_7    };
constexpr TextureUnit TEXTURE_UNIT_8    = { TEXTURE_UNIT_ID_8,    TEXTURE_UNIT_CODE_8    };
constexpr TextureUnit TEXTURE_UNIT_9    = { TEXTURE_UNIT_ID_9,    TEXTURE_UNIT_CODE_9    };
constexpr TextureUnit TEXTURE_UNIT_10   = { TEXTURE_UNIT_ID_10,   TEXTURE_UNIT_CODE_10   };
constexpr TextureUnit TEXTURE_UNIT_11   = { TEXTURE_UNIT_ID_11,   TEXTURE_UNIT_CODE_11   };
constexpr TextureUnit TEXTURE_UNIT_12   = { TEXTURE_UNIT_ID_12,   TEXTURE_UNIT_CODE_12   };
constexpr TextureUnit TEXTURE_UNIT_13   = { TEXTURE_UNIT_ID_13,   TEXTURE_UNIT_CODE_13   };
constexpr TextureUnit TEXTURE_UNIT_14   = { TEXTURE_UNIT_ID_14,   TEXTURE_UNIT_CODE_14   };
constexpr TextureUnit TEXTURE_UNIT_15   = { TEXTURE_UNIT_ID_15,   TEXTURE_UNIT_CODE_15   };
constexpr TextureUnit TEXTURE_UNIT_16   = { TEXTURE_UNIT_ID_16,   TEXTURE_UNIT_CODE_16   };
constexpr TextureUnit TEXTURE_UNIT_17   = { TEXTURE_UNIT_ID_17,   TEXTURE_UNIT_CODE_17   };
constexpr TextureUnit TEXTURE_UNIT_18   = { TEXTURE_UNIT_ID_18,   TEXTURE_UNIT_CODE_18   };
constexpr TextureUnit TEXTURE_UNIT_19   = { TEXTURE_UNIT_ID_19,   TEXTURE_UNIT_CODE_19   };
constexpr TextureUnit TEXTURE_UNIT_20   = { TEXTURE_UNIT_ID_20,   TEXTURE_UNIT_CODE_20   };
constexpr TextureUnit TEXTURE_UNIT_21   = { TEXTURE_UNIT_ID_21,   TEXTURE_UNIT_CODE_21   };
constexpr TextureUnit TEXTURE_UNIT_22   = { TEXTURE_UNIT_ID_22,   TEXTURE_UNIT_CODE_22   };
constexpr TextureUnit TEXTURE_UNIT_23   = { TEXTURE_UNIT_ID_23,   TEXTURE_UNIT_CODE_23   };
constexpr TextureUnit TEXTURE_UNIT_24   = { TEXTURE_UNIT_ID_24,   TEXTURE_UNIT_CODE_24   };
constexpr TextureUnit TEXTURE_UNIT_25   = { TEXTURE_UNIT_ID_25,   TEXTURE_UNIT_CODE_25   };
constexpr TextureUnit TEXTURE_UNIT_26   = { TEXTURE_UNIT_ID_26,   TEXTURE_UNIT_CODE_26   };
constexpr TextureUnit TEXTURE_UNIT_27   = { TEXTURE_UNIT_ID_27,   TEXTURE_UNIT_CODE_27   };
constexpr TextureUnit TEXTURE_UNIT_28   = { TEXTURE_UNIT_ID_28,   TEXTURE_UNIT_CODE_28   };
constexpr TextureUnit TEXTURE_UNIT_29   = { TEXTURE_UNIT_ID_29,   TEXTURE_UNIT_CODE_29   };
constexpr TextureUnit TEXTURE_UNIT_30   = { TEXTURE_UNIT_ID_30,   TEXTURE_UNIT_CODE_30   };
constexpr TextureUnit TEXTURE_UNIT_31   = { TEXTURE_UNIT_ID_31,   TEXTURE_UNIT_CODE_31   };


struct Texture
{
    // NOTE: config
    TextureWrapping     xWrapping;
    TextureWrapping     yWrapping;
    vec4                borderColor;
    TextureMinFiltering minFilter;
    TextureMagFiltering magFilter;
    TextureUnit         unit;

    // NOTE: data
    u32 id;
    i32 width;
    i32 height;
    i32 channels;
};

void TextureBind(Texture* texture)
{
    glActiveTexture(texture->unit.code);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void TextureUnbind(Texture* /*texture*/)
{
    // TODO: to be implemented

    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureInit(Texture* texture, const c8* path)
{
    // NOTE: always force 4 channels
    texture->channels = 4;
    stbi_set_flip_vertically_on_load(true);
    u8* data = stbi_load(
        path,
        &texture->width, &texture->height, NULL,
        texture->channels
    );
    ExAssertFmt(data, "Failed to load image '%s'", path);

    glGenTextures(1, &texture->id);

    TextureBind(texture);

    // NOTE: set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->xWrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->yWrapping);

    // NOTE: set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->magFilter);

    // NOTE: upload texture data
    // NOTE: we use GL_RGBA because we always use 4 channels
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        texture->width, texture->height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    // NOTE: generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    TextureUnbind(texture);

    stbi_image_free(data);
}

void TextureFini(Texture* /*texture*/)
{
    // TODO: to be implemented
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

void MaterialFini(Material* material)
{
    glDeleteProgram(material->program);
}

i32 MaterialGetUniformIndex(Material* material, const c8* name)
{
    i32 index = glGetUniformLocation(material->program, name);
    ExAssertFmt(index != -1, "There is no uniform named '%s'");
    return index;
}

void MaterialSetI32(Material* material, const c8* name, i32 value)
{
    glUniform1i(MaterialGetUniformIndex(material, name), value);
}

void MaterialSet3F32(Material* material, const c8* name, f32 a, f32 b, f32 c)
{
    glUniform3f(MaterialGetUniformIndex(material, name), a, b, c);
}

struct Context
{
    Mesh quadMesh;
    Material defaultMaterial;
    Texture texture;
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
        MaterialInit(&ctx->defaultMaterial, "data/shaders/texture.vert", "data/shaders/texture.frag");
    }

    // NOTE: load a texture
    {
        ctx->texture.xWrapping = TEXTURE_WRAP_CLAMP_TO_EDGE;
        ctx->texture.yWrapping = TEXTURE_WRAP_CLAMP_TO_EDGE;
        ctx->texture.minFilter = TEXTURE_MIN_FILTER_LINEAR_MIPMAP_LINEAR;
        ctx->texture.magFilter = TEXTURE_MAG_FILTER_LINEAR;
        ctx->texture.unit = TEXTURE_UNIT_0;
        TextureInit(&ctx->texture, "data/textures/lena.png");
    }
}

void Shutdown(Context* ctx)
{
    TextureFini(&ctx->texture);
    MaterialFini(&ctx->defaultMaterial);
    MeshFini(&ctx->quadMesh);
}

void MainLoop(Context* ctx)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    TextureBind(&ctx->texture);
    glUseProgram(ctx->defaultMaterial.program);
    MaterialSetI32(&ctx->defaultMaterial, "texture0", ctx->texture.unit.id);
    //MaterialSet3F32(&ctx->defaultMaterial, "color", 1.0f, 1.0f, 0.0f);

    glBindVertexArray(ctx->quadMesh.vao);
    glDrawElements(GL_TRIANGLES, (i32)(ctx->quadMesh.indexCount), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
    TextureUnbind(&ctx->texture);
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

    Shutdown(&ctx);

    glfwTerminate();

    return 0;
}
