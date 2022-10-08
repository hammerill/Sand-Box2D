#pragma once

#include "PhysicsObj.h"

class PlatformEntity: public PhysicsObj
{
private:
    float x1, y1, x2, y2;

    b2EdgeShape platformShape;

public:
    PlatformEntity(b2World* world, float x1_plat, float y1_plat, float x2_plat, float y2_plat);

    void Reset();
    void Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom);
};
