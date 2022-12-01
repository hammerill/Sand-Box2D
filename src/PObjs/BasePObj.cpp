#include "BasePObj.h"

void BasePObj::Register(b2World* world, SDL_Renderer* renderer)
{
    BasePObj::body = world->CreateBody(&(BasePObj::bodyDef));
    BasePObj::body->SetLinearVelocity(BasePObj::vel);
    BasePObj::body->CreateFixture(&(BasePObj::fixtureDef));
}

b2Body* BasePObj::GetBody()
{
    return BasePObj::body;
}
