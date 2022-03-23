#pragma once

#include "Ex/Engine/Modules/GameEventsListener.h"

class EngineStateModule : public GameModuleEventsListener
{
private:
    bool isRunning;
    bool isPaused;
public:
    void Setup();

    bool IsRunning();
    bool IsPaused();

    // Game module events listener ---------------------------------------------
    void OnQuit();
};
