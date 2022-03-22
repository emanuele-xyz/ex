#include "Ex/Engine/Modules/Game.h"

#include "Ex/Systems/Memory.h"

void GameModule::Init()
{
    // TODO: to be implemented

    // TODO: use a Vec
    listenersCount = 16;
    listeners = ExArrayAlloc<GameModuleEventsListener*>(listenersCount);
}

void GameModule::Fini()
{
    ExFree(listeners);
    listenersCount = 0;

    // TODO: to be implemented
}

void GameModule::Update()
{
    // TODO: to be implemented
}

void GameModule::Render()
{
    // TODO: to be implemented
}

void GameModule::EmitQuitEvent()
{
    for (u32 i = 0; i < listenersCount; i++)
    {
        if (listeners[i] != nullptr)
        {
            listeners[i]->OnQuit();
        }
    }
}

void GameModule::OnCloseRequest()
{
    EmitQuitEvent();
}
