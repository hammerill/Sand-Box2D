#include "PlatformEntity.h"

PlatformEntity::PlatformEntity(float x1_plat, float y1_plat, float x2_plat, float y2_plat)
{
    PlatformEntity::world = world;
    PlatformEntity::x1 = x1_plat;
    PlatformEntity::y1 = y1_plat;
    PlatformEntity::x2 = x2_plat;
    PlatformEntity::y2 = y2_plat;

    b2Vec2 start_point = b2Vec2(PlatformEntity::x1, PlatformEntity::y1);
    b2Vec2 end_point = b2Vec2(PlatformEntity::x2, PlatformEntity::y2);
    
    PlatformEntity::platformShape.SetTwoSided(start_point, end_point);

    PlatformEntity::fixtureDef.shape = &(PlatformEntity::platformShape);
}

void PlatformEntity::Register(b2World* world, SDL_Renderer* renderer)
{
    PlatformEntity::body = world->CreateBody(&(PlatformEntity::bodyDef));
    PlatformEntity::body->CreateFixture(&(PlatformEntity::fixtureDef));
}

void PlatformEntity::Reset() {}
void PlatformEntity::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

    SDL_RenderDrawLine(renderer, 
    (PlatformEntity::x1 * zoom) + x_offset, 
    (PlatformEntity::y1 * zoom) + y_offset, 
    (PlatformEntity::x2 * zoom) + x_offset, 
    (PlatformEntity::y2 * zoom) + y_offset);
}
