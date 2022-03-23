#include "Ex/Engine/Engine.h"

#include "Ex/Systems/Logger.h"

void Engine::Init()
{
    // NOTE: initialize modules
    engineState.Setup();
    settings.Load();
    window.Init(settings.windowWidth, settings.windowHeight, settings.appName);
    graphics.Init(window.GetHandle());
    input.Init();
    game.Init();

    // NOTE: wire up modules
    window.AddListener(&graphics);
    window.AddListener(&game);

    game.AddListener(&engineState);
}

void Engine::Fini()
{
    game.Fini();
    input.Fini();
    graphics.Fini();
    window.Fini();
    settings.Save();
}

void Engine::Run()
{
    while (engineState.IsRunning())
    {
        window.PollEvents();

        if (engineState.IsPaused())
        {
            // NOTE: do nothing
        }
        else
        {
            game.Tick();
        }
    }
}
