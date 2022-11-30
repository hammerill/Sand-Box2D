#include "BasePObj.h"

void BasePObj::Register(b2World* world, SDL_Renderer* renderer)
{
    BasePObj::body = world->CreateBody(&(BasePObj::bodyDef));
    BasePObj::body->CreateFixture(&(BasePObj::fixtureDef));
    
    if (BasePObj::pathToTexture != nullptr)
        BasePObj::LoadTexture(renderer);
}

void BasePObj::LoadTexture(SDL_Renderer* renderer)
{
    BasePObj::texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(BasePObj::pathToTexture));
}

b2Body* BasePObj::GetBody()
{
    return BasePObj::body;
}
