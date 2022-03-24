#pragma once

#include "Ex/Core.h"
#include "Ex/App/InputEvent.h"
#include "Ex/Game/Graphics.h"

class Game
{
private:
    Graphics graphics;
    bool hasFinished;
public:
    void Init(void* windowHandle);
    void Fini();
    void ProcessInput(u32 inputEventCount, InputEvent* inputEvents);
    bool Update();
};
