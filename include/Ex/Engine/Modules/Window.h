#pragma once

#include "Ex/Core.h"
#include "Ex/Engine/Modules/Settings.h"

typedef void (WindowModuleResizeCallbackFn)(i32 width, i32 height);

class WindowModule
{
public:
    void* handle;
    i32   width;
    i32   height;
    f32   aspect;

    WindowModuleResizeCallbackFn* resizeCallbackFn;

public:
    void Init(SettingsModule* settings);
    void Fini();
};
