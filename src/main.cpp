#include "WorldManager.h"

#ifdef Linux
const bool isLinux = true;
#else
const bool isLinux = false;
#endif

#ifdef Vita
int WINDOW_WIDTH = 960;
int WINDOW_HEIGHT = 544;
#else
int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;
#endif

int main(int argv, char** args)
{    
    WorldManager* wm = new WorldManager(WINDOW_WIDTH, WINDOW_HEIGHT, "assets/fonts/PressStart2P-vaV7.ttf", isLinux, isLinux ? "assets/img/icon.png" : nullptr);

    Level level;
    level.LoadFile("./assets", "default_level/default_level.json");

    wm->LoadLevel(level);

    wm->Cycle();

    return 0;
}
