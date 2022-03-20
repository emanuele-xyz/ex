#pragma once

#include "Ex/Engine/Modules/Settings.h"
#include "Ex/Engine/Modules/Window.h"
#include "Ex/Engine/Modules/Graphics.h"
#include "Ex/Engine/Modules/Input.h"

struct EngineServices
{
    SettingsModule* settings;
    WindowModule*   window;
    GraphicsModule* graphics;
    InputModule*    input;
};
