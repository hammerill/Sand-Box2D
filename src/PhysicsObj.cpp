#include "PhysicsObj.h"

void PhysicsObj::Register(b2World* world, SDL_Renderer* renderer)
{
    PhysicsObj::body = world->CreateBody(&(PhysicsObj::bodyDef));
    PhysicsObj::body->SetLinearVelocity(PhysicsObj::vel);
    PhysicsObj::body->CreateFixture(&(PhysicsObj::fixtureDef));
    
    if (PhysicsObj::pathToTexture != nullptr)
        PhysicsObj::LoadTexture(renderer);
}

void PhysicsObj::LoadTexture(SDL_Renderer* renderer)
{
    PhysicsObj::texture = SDL_CreateTextureFromSurface(renderer, IMG_Load(PhysicsObj::pathToTexture));
}

b2Body* PhysicsObj::getBody()
{
    return PhysicsObj::body;
}
