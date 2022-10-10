#include "PhysicsObj.h"

PhysicsObj::~PhysicsObj()
{
    SDL_DestroyTexture(PhysicsObj::texture);
}

void PhysicsObj::LoadTexture(SDL_Renderer* renderer)
{
    PhysicsObj::texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(PhysicsObj::pathToTexture));
}

b2Body* PhysicsObj::getBody()
{
    return PhysicsObj::body;
}
