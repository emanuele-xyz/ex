#pragma once

class EngineState
{
private:
    bool isRunning;
    bool isPaused;
public:
    bool IsRunning();
    bool IsPaused();
};
