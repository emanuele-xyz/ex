#pragma once

#include "Ex/Engine/Modules/Settings.h"
#include "Ex/Engine/Modules/Window.h"
#include "Ex/Engine/Modules/Input.h"
#include "Ex/Engine/Modules/Graphics.h"
#include "Ex/Engine/Modules/Game.h"

struct EngineServices
{
    SettingsModule* settings;
    WindowModule*   window;
    InputModule*    input;
    GraphicsModule* graphics;
};

class Engine
{
private:
    SettingsModule settings;
    WindowModule   window;
    InputModule    input;
    GraphicsModule graphics;

    EngineServices services;

    GameModule     game;

public:
    void Init();
    void Fini();
    void Run();
};
