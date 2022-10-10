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
int SCREEN_WIDTH = 960;
int SCREEN_HEIGHT = 544;
#else
int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
#endif

int main(int argv, char** args)
{    
    WorldManager* wm = new WorldManager(SCREEN_WIDTH, SCREEN_HEIGHT, isLinux);

    wm->addObject(new BoxEntity(wm->world, wm->renderer, "assets/img/box.png", 0, 0, 2, 2, 0));
    wm->addObject(new BoxEntity(wm->world, wm->renderer, "assets/img/box.png", 2, 0, 2, 1, 45.0f));
    wm->addObject(new BoxEntity(wm->world, wm->renderer, "assets/img/box.png", 5, 0, 0.25, 1.5, 45.0f));
    wm->addObject(new BoxEntity(wm->world, wm->renderer, "assets/img/box.png", 5.5, 0, 0.25, 0.25, 45.0f));

    wm->addObject(new PlatformEntity(wm->world, 0, 6, 8, 6));

    wm->Cycle();

    return 0;
}
