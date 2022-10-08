#pragma once

#include "PhysicsObj.h"

class BoxEntity: public PhysicsObj
{
private:
    float x, y, w, h, angle;

    SDL_Rect box;
    b2PolygonShape boxShape;

public:
    BoxEntity(b2World* world, SDL_Renderer* renderer, const char* path_to_texture, float x_box, float y_box, float w_box, float h_box, float angle_box);
    ~BoxEntity();

    void Reset();
    void Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom);
};
