#pragma once

#include "Ex/Engine/Services.h"
#include "Ex/Engine/Modules/Game.h"

class Engine
{
private:
    SettingsModule settings;
    WindowModule   window;
    GraphicsModule graphics;
    InputModule    input;

    EngineServices services;

    GameModule     game;

    bool isRunning;
    bool isPaused;

public:
    void Init();
    void Fini();
    void Run();
};
