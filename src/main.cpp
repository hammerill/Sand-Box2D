#include "GameManager.h"

#if defined(Windows)
const char* settingsFile = "./settings.json";
const char* defSettingsFile = "./assets/default_settings/windows.json";
#elif defined(Linux)
const char* settingsFile = "./settings.json";
const char* defSettingsFile = "./assets/default_settings/linux.json";
#elif defined(Vita)
const char* settingsFile = "ux0:/data/Sand-Box2D/settings.json";
const char* defSettingsFile = "./assets/default_settings/vita.json";
#endif

int main(int argv, char** args)
{    
    GameManager* gm = new GameManager(settingsFile, defSettingsFile);
    gm->Cycle();

    return 0;
}
