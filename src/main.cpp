#include "Controls.h"
#include "BoxEntity.h"
#include "PlatformEntity.h"
#include "CircleEntity.h"
#include "WorldManager.h"

#include <chrono>
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
        std::this_thread::sleep_for(std::chrono::seconds(1));

        wm->addObject(new BoxEntity(texture, 6 + 0.125, 0, 0.25, 0.25, 0, 10, 10));
        wm->addObject(new BoxEntity(texture, 6 - 0.125, 0, 0.25, 0.25, 0, -10, 10));
    }
}

int main(int argv, char** args)
{    
    WorldManager* wm = new WorldManager(WINDOW_WIDTH, WINDOW_HEIGHT, isLinux);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(wm->getRenderer(), IMG_Load("assets/img/box.png"));

    wm->addObject(new PlatformEntity(2, 7, 10, 7)); // BOTTOM __ /

    wm->addObject(new PlatformEntity(1, 1, 2, 7)); // LEFT \ /
    wm->addObject(new PlatformEntity(10, 7, 11, 1)); // RIGHT / /

    wm->addObject(new CircleEntity(6, 2, 0.5, 5, 5));

    std::thread creater(objCreateCycle, wm, texture);

    wm->Cycle();

    isRunning = false;

    return 0;
}
