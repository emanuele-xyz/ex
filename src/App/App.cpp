#include "Ex/App/App.h"

void App::Init()
{
    state.Init();
    settings.Load();
    window.Init(settings.windowWidth, settings.windowHeight, settings.appName);
    input.Init(window.GetHandle());
    game.Init(window.GetHandle());
}

void App::Fini()
{
    game.Fini();
    input.Fini();
    window.Fini();
    settings.Save();
    state.Fini();
}

void App::Run()
{
    while (state.IsRunning())
    {
        // NOTE: process window events and get new input
        window.PollEvents();

        if (state.IsPaused())
        {
            // NOTE: do nothing
        }
        else
        {
            u32 inputEventCount = input.GetEventCount();
            InputEvent* inputEvents = input.GetEvents();
            game.ProcessInput(inputEventCount, inputEvents);

            bool hasGameFinished = game.Update();
            if (hasGameFinished) state.Quit();

            window.SwapBuffers();
        }

        input.ClearEvents();
    }
}
