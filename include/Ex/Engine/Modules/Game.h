#pragma once

#include "Ex/Engine/Services.h"

class GameModule
{
private:
    EngineServices* services;

public:
    void Init(EngineServices* s);
    void Fini();
    void Update();
    void Render();
};
