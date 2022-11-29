#include "GameManager.h"

#if defined(Windows)
const char* SETTINGS_FILE = "./settings.json";
const char* DEF_SETTINGS_FILE = "./assets/default_settings/windows.json";
#elif defined(Linux)
const char* SETTINGS_FILE = "./settings.json";
const char* DEF_SETTINGS_FILE = "./assets/default_settings/linux.json";
#elif defined(Vita)
const char* SETTINGS_FILE = "ux0:/data/Sand-Box2D/settings.json";
const char* DEF_SETTINGS_FILE = "./assets/default_settings/vita.json";
#endif

int main(int argv, char** args)
{    
    GameManager* gm = new GameManager(SETTINGS_FILE, DEF_SETTINGS_FILE);
    gm->Cycle();

    return 0;
}
