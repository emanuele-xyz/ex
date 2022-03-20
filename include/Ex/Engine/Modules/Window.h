#pragma once

#include "Ex/Core.h"

class WindowModule
{
public:
    void* handle;
    i32   width;
    i32   height;
    f32   aspect;

public:
    void Init();
    void Fini();
};
