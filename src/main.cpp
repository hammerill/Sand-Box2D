#include "GameManager.h"

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
    GameManager* gm = new GameManager("./settings.json", "./assets/default_settings/windows.json");

    gm->Cycle();

    return 0;
}
