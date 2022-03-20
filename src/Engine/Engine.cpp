#include "Ex/Engine/Engine.h"

#include "Ex/Systems/Logger.h"

void Engine::Init()
{
    settings.Load();
    window.Init(&settings);
    graphics.Init(&window);
    input.Init();

    services.settings = &settings;
    services.window   = &window;
    services.graphics = &graphics;
    services.input    = &input;

    game.Init(&services);
}

void Engine::Fini()
{
    game.Fini();

    services.settings = nullptr;
    services.window = nullptr;
    services.graphics = nullptr;
    services.input = nullptr;

    input.Fini();
    graphics.Fini();
    window.Fini();
    settings.Save();
}

void Engine::Run()
{
    ExLog("Running ...");
}
