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

float randFloat(float min, float max) { return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min))); }

void objCreateCycle(WorldManager* wm, SDL_Texture* texture)
{
    while (true)
    {
        float min = 0.1;
        float max = 2.0;

        BoxEntity* box = new BoxEntity(texture, 7, 0, randFloat(min, max), randFloat(min, max), 0, 10, 10);
        box->isMarkedToDelete = true;
        wm->addObject(box);
        
        CircleEntity* circle = new CircleEntity(5, 0, randFloat(min, max) / 3.0, -10, 10);
        circle->isMarkedToDelete = true;
        wm->addObject(circle);

        std::this_thread::sleep_for(std::chrono::seconds(4 + std::rand() % 4));
    }
}

int main(int argv, char** args)
{    
    WorldManager* wm = new WorldManager(WINDOW_WIDTH, WINDOW_HEIGHT, "assets/fonts/PressStart2P-vaV7.ttf", isLinux, isLinux ? "assets/img/icon.png" : nullptr);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(wm->getRenderer(), IMG_Load("assets/img/box.png"));

    wm->addObject(new PlatformEntity(2, 7, 10, 7)); // BOTTOM __ /

    wm->addObject(new PlatformEntity(1, 1, 2, 7)); // LEFT \ /
    wm->addObject(new PlatformEntity(10, 7, 11, 1)); // RIGHT / /

    std::thread creater(objCreateCycle, wm, texture);

    wm->Cycle();

    creater.detach();

    return 0;
}
