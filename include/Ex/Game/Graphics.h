#pragma once

#include "Ex/Core.h"

struct Mesh
{
    u32 id;
};

class Graphics
{
public:
    void Init(void* windowHandle);
    void Fini();

    Mesh LoadMesh(const char* path);
    void UnloadMesh(Mesh mesh);

    void ClearColorBuffer(f32 r, f32 g, f32 b, f32 a);
    void ResizeFramebuffer(i32 width, i32 height);
};
