#pragma once

#include <box2d/box2d.h>
#include <vector>

#include "PhysicsObj.h"

class WorldManager
{
private:
    std::vector<PhysicsObj*> objects;

    int SCREEN_WIDTH, SCREEN_HEIGHT;
    bool fpsCorrection;

    SDL_Window* window;

    void initVideo();

    unsigned int a, b = 0;
    double delta = 0;

    float x_offset = 0, y_offset = 0, zoom = 80;
    float move_speed, zoom_speed;

public:
    WorldManager(int SCREEN_WIDTH, int SCREEN_HEIGHT, bool fpsCorrection = false, float move_speed = 10, float zoom_speed = 1);
    ~WorldManager();

    void addObject(PhysicsObj* obj);
    void deleteObject(int index);

    // Returns running status (true - still running; false - stop)
    bool Step();
    
    void Render();
    void Cycle();
    
    b2World* world;
    SDL_Renderer* renderer;
};
