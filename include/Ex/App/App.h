#pragma once

#include "Ex/App/State.h"
#include "Ex/App/Settings.h"
#include "Ex/App/Window.h"
#include "Ex/App/Input.h"

#include "Ex/Game/Game.h"

class App
{
private:
    State    state;
    Settings settings;
    Window   window;
    Input    input;
    Game     game;

public:
    void Init();
    void Fini();
    void Run();
};
