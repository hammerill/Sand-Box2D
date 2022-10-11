#include "PhysicsObj.h"

PhysicsObj::~PhysicsObj()
{
    if (PhysicsObj::pathToTexture != nullptr)
    {
        SDL_DestroyTexture(PhysicsObj::texture);
        delete[] PhysicsObj::pathToTexture;
    }

    PhysicsObj::world->DestroyBody(PhysicsObj::body);

    delete[] PhysicsObj::world;
}

void PhysicsObj::LoadTexture(SDL_Renderer* renderer)
{
    PhysicsObj::texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(PhysicsObj::pathToTexture));
}

b2Body* PhysicsObj::getBody()
{
    return PhysicsObj::body;
}
