#include "Controls.h"
#include "BoxEntity.h"
#include "PlatformEntity.h"
#include "WorldManager.h"

#include <thread>

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

bool isRunning = true;

void objCreateCycle(WorldManager* wm, SDL_Texture* texture)
{
    while (isRunning)
    {
        _sleep(10); //100ms delay. 10Hz (10 times per second).
        wm->addObject(new BoxEntity(texture, 6, 8, 0.25, 0.25, 0));
    }
}

int main(int argv, char** args)
{    
    WorldManager* wm = new WorldManager(WINDOW_WIDTH, WINDOW_HEIGHT, isLinux);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(wm->getRenderer(), IMG_Load("assets/img/box.png"));

    wm->addObject(new PlatformEntity(2, 7, 10, 7)); // BOTTOM __ /

    wm->addObject(new PlatformEntity(1, 1, 2, 7)); // LEFT \ /
    wm->addObject(new PlatformEntity(10, 7, 11, 1)); // RIGHT / /

    std::thread creater(objCreateCycle, wm, texture);

    wm->Cycle();

    isRunning = false;

    return 0;
}
