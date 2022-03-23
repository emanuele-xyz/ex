#include "Ex/Engine/Modules/EngineState.h"

void EngineStateModule::Setup()
{
    isRunning = true;
    isPaused = false;
}

bool EngineStateModule::IsRunning()
{
    return isRunning;
}

bool EngineStateModule::IsPaused()
{
    return isPaused;
}

void EngineStateModule::OnQuit()
{
    isRunning = false;
}
