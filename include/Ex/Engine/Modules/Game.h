#pragma once

#include "Ex/Core.h"
#include "Ex/Engine/Modules/WindowEventsListener.h"
#include "Ex/Engine/Modules/GameEventsListener.h"

class GameModule : public WindowModuleEventsListener
{
private:
    u32 listenersCount;
    GameModuleEventsListener** listeners;
public:
    // Main Methods ------------------------------------------------------------
    void Init();
    void Fini();
    void Update();
    void Render();

    // Events Emitter ----------------------------------------------------------
    void EmitQuitEvent();

    // Events Listener ---------------------------------------------------------
    void OnCloseRequest();
};
