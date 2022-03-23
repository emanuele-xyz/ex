#pragma once

#include "Ex/Engine/Modules/EngineState.h"
#include "Ex/Engine/Modules/Settings.h"
#include "Ex/Engine/Modules/Window.h"
#include "Ex/Engine/Modules/Graphics.h"
#include "Ex/Engine/Modules/Input.h"
#include "Ex/Engine/Modules/Game.h"

class Engine
{
private:
    EngineStateModule engineState;
    SettingsModule    settings;
    WindowModule      window;
    GraphicsModule    graphics;
    InputModule       input;
    GameModule        game;

public:
    void Init();
    void Fini();
    void Run();
};
