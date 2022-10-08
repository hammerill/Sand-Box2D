#include "PhysicsObj.h"

PhysicsObj::~PhysicsObj()
{
    SDL_DestroyTexture(PhysicsObj::texture);
}

void PhysicsObj::LoadTexture(SDL_Renderer* renderer, const char* path_to_texture)
{
    PhysicsObj::texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(path_to_texture));
}

b2Body* PhysicsObj::getBody()
{
    return PhysicsObj::body;
}
