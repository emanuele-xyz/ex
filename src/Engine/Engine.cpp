#include "Ex/Engine/Engine.h"

#include "Ex/Systems/Logger.h"

void Engine::Init()
{
    settings.Load();
    window.Init(settings.windowWidth, settings.windowHeight, settings.appName);
    graphics.Init(&window);
    input.Init();

    services.settings = &settings;
    services.window   = &window;
    services.graphics = &graphics;
    services.input    = &input;

    game.Init(&services);

    isRunning = true;
    isPaused = false;
}

void Engine::Fini()
{
    game.Fini();

    services.settings = nullptr;
    services.window   = nullptr;
    services.graphics = nullptr;
    services.input    = nullptr;

    input.Fini();
    graphics.Fini();
    window.Fini();
    settings.Save();
}

void Engine::Run()
{
//    isRunning = window.ShouldClose(); // TODO: it doesn't make any sense for the engine state to be set by the window module
//    isPaused = false; // TODO: check how to update this
//
//    while (isRunning)
//    {
//        if (isPaused)
//        {
//            // NOTE: do nothing
//        }
//        else
//        {
//
//        }
//    }
}
