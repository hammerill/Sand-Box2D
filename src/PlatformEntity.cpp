#include "PlatformEntity.h"

PlatformEntity::PlatformEntity(b2World* world, float x1_plat, float y1_plat, float x2_plat, float y2_plat)
{
    PlatformEntity::world = world;
    PlatformEntity::x1 = x1_plat;
    PlatformEntity::y1 = y1_plat;
    PlatformEntity::x2 = x2_plat;
    PlatformEntity::y2 = y2_plat;

    b2Vec2 start_point = b2Vec2(PlatformEntity::x1, PlatformEntity::y1);
    b2Vec2 end_point = b2Vec2(PlatformEntity::x2, PlatformEntity::y2);

    PlatformEntity::bodyDef.type = b2_staticBody;
    PlatformEntity::bodyDef.angle = 0; 
    PlatformEntity::bodyDef.position.Set(PlatformEntity::x1, PlatformEntity::y1);

    PlatformEntity::body = PlatformEntity::world->CreateBody(&bodyDef);
    
    PlatformEntity::platformShape.SetTwoSided(start_point, end_point);

    PlatformEntity::fixtureDef.shape = &(PlatformEntity::platformShape);
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
