#pragma once

#include "Ex/Core.h"

class WindowModule
{
private:
    void* handle;
    i32   width;
    i32   height;
    f32   aspect;

public:
    void Init();
    void Fini();

    // TODO: temporary
    #pragma warning (push)
    #pragma warning (disable : 4514)
    void* GetHandle() { return handle; };
    i32   GetWidth()  { return width; };
    i32   GetHeight() { return height; };
    f32   GetAspect() { return aspect; };
    #pragma warning (pop)
};
