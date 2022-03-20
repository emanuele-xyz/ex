#include "Ex/Engine/Modules/Settings.h"

void SettingsModule::Load()
{
    // TODO: load settings from default.json or custom.json file
    appName = "Project Ex";

    windowWidth = 1280;
    windowHeight = 720;
}

void SettingsModule::Save()
{
    // TODO: write current settings to custom.json
}
