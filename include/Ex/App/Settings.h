#pragma once

#include "Ex/Core.h"

class Settings
{
public:
    i32 windowWidth;
    i32 windowHeight;
    char* appName;

public:
    void Load();
    void Save();
};
