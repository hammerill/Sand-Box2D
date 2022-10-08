#include "PhysicsObj.h"

void PhysicsObj::SetSprite(SDL_Renderer* renderer, SDL_Surface* tmp_sprites)
{
    PhysicsObj::texture = SDL_CreateTextureFromSurface(renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
}

b2Body* PhysicsObj::getBody()
{
    return PhysicsObj::body;
}
