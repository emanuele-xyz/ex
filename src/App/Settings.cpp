#include "Ex/App/Settings.h"

void Settings::Load()
{
    // TODO: load settings from default.json or custom.json file
    appName = "Project Ex";

    windowWidth = 1280;
    windowHeight = 720;
}

void Settings::Save()
{
    // TODO: write current settings to custom.json
}
