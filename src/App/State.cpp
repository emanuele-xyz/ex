#include "Ex/App/State.h"

void State::Init()
{
    isRunning = true;
    isPaused = false;
}

void State::Fini()
{
}

bool State::IsRunning()
{
    return isRunning;
}

bool State::IsPaused()
{
    return isPaused;
}

void State::Quit()
{
    ExAssert(isRunning);

    isRunning = false;
}
