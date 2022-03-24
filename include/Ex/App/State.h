#pragma once

#include "Ex/Core.h"

class State
{
private:
    bool isRunning;
    bool isPaused;
public:
    void Init();
    void Fini();

    bool IsRunning();
    bool IsPaused();
    void Quit();
};
