#include "Ex/Engine/Modules/Game.h"

void GameModule::Init(EngineServices* s)
{
    services = s;
}

void GameModule::Fini()
{
    services = nullptr;
}

void GameModule::Update()
{
    // TODO: to be implemented
}

void GameModule::Render()
{
    // TODO: to be implemented
}
