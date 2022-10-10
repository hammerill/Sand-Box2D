#include "Controls.h"
#include "BoxEntity.h"
#include "PlatformEntity.h"
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
    WorldManager* wm = new WorldManager(WINDOW_WIDTH, WINDOW_HEIGHT, isLinux);

    wm->addObject(new PlatformEntity(0, 6, 8, 6));

    wm->addObject(new BoxEntity("assets/img/box.png", 0, 0, 2, 2, 0));
    wm->addObject(new BoxEntity("assets/img/box.png", 2, 0, 2, 1, 45.0f));
    wm->addObject(new BoxEntity("assets/img/box.png", 5, 0, 0.25, 1.5, 45.0f));
    wm->addObject(new BoxEntity("assets/img/box.png", 5.5, 0, 0.25, 0.25, 45.0f));

    wm->Cycle();

    return 0;
}
