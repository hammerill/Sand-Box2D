#include "WorldManager.h"
#include "Controls.h"

WorldManager::WorldManager(int SCREEN_WIDTH, int SCREEN_HEIGHT, bool fpsCorrection, float move_speed, float zoom_speed)
{
    WorldManager::SCREEN_WIDTH = SCREEN_WIDTH;
    WorldManager::SCREEN_HEIGHT = SCREEN_HEIGHT;

    WorldManager::fpsCorrection = fpsCorrection;

    WorldManager::move_speed = move_speed;
    WorldManager::zoom_speed = zoom_speed;

    b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
    WorldManager::world = new b2World(gravity);

    WorldManager::objects = std::vector<PhysicsObj*>();

    WorldManager::initVideo();
}
WorldManager::~WorldManager()
{
    for (int i = objects.size() - 1; i >= 0; i--)
    {
        WorldManager::deleteObject(i);
    }
}

void WorldManager::initVideo()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    WorldManager::window = SDL_CreateWindow("Box2D", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WorldManager::SCREEN_WIDTH, WorldManager::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    WorldManager::renderer = SDL_CreateRenderer(WorldManager::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(WorldManager::renderer, WorldManager::SCREEN_WIDTH, WorldManager::SCREEN_HEIGHT);
}

void WorldManager::addObject(PhysicsObj* obj)
{
    WorldManager::objects.push_back(obj);
}
void WorldManager::deleteObject(int index)
{
    WorldManager::world->DestroyBody(WorldManager::objects[index]->getBody());
    delete[] &WorldManager::objects[index];
}

bool WorldManager::Step()
{
    if (WorldManager::fpsCorrection)
        WorldManager::b = WorldManager::a;

    Ctrl::Check();

    if (Ctrl::getExit())
    {
        return false;
    }
    if (Ctrl::getReset())
    {
        for (size_t i = 0; i < WorldManager::objects.size(); i++)
        {
            WorldManager::objects[i]->Reset();
        }
    }

    if (Ctrl::getUp())      { WorldManager::y_offset += WorldManager::move_speed; }
    if (Ctrl::getRight())   { WorldManager::x_offset -= WorldManager::move_speed; }
    if (Ctrl::getDown())    { WorldManager::y_offset -= WorldManager::move_speed; }
    if (Ctrl::getLeft())    { WorldManager::x_offset += WorldManager::move_speed; }
    
    if (Ctrl::getPlus())            { WorldManager::zoom += WorldManager::zoom_speed; }
    if (Ctrl::getMinus()
        && WorldManager::zoom > 0)  { WorldManager::zoom -= WorldManager::zoom_speed; }

    WorldManager::world->Step(1.0f / 60.0f, 6.0f, 2.0f);

    return true;
}

void WorldManager::Render()
{
    SDL_SetRenderDrawColor(WorldManager::renderer, 0x32, 0x32, 0x32, 0);
    SDL_RenderClear(WorldManager::renderer);

    for (size_t i = 0; i < WorldManager::objects.size(); i++)   
    {
        WorldManager::objects[i]->Render(   WorldManager::renderer, 
                                            WorldManager::x_offset, 
                                            WorldManager::y_offset, 
                                            WorldManager::zoom);
    }

    SDL_RenderPresent(renderer);
}

void WorldManager::Cycle()
{
    bool isRunning = true;

    if (WorldManager::fpsCorrection)
    {
        while (isRunning)
        {
            WorldManager::a = SDL_GetTicks();
            WorldManager::delta = WorldManager::a - WorldManager::b;

            if (delta > 1000/60.0)
            {
                isRunning = WorldManager::Step();
                WorldManager::Render();
            }
        }
    }
    else
    {
        while (isRunning)
        {
            isRunning = WorldManager::Step();
            WorldManager::Render();
        }
    }
}
