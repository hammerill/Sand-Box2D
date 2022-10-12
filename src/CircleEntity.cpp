#include "CircleEntity.h"

CircleEntity::CircleEntity(float x, float y, float radius, float vel_x, float vel_y, Uint32 color)
{
    CircleEntity::x = x;
    CircleEntity::y = y;
    CircleEntity::radius = radius;
    CircleEntity::color = color;

    CircleEntity::vel.Set(vel_x, vel_y);

    CircleEntity::bodyDef.type = b2_dynamicBody;
    CircleEntity::bodyDef.position.Set(CircleEntity::x, CircleEntity::y);
    CircleEntity::bodyDef.angle = 0;

    CircleEntity::circleShape.m_radius = radius;

    CircleEntity::fixtureDef.shape = &(CircleEntity::circleShape);
    CircleEntity::fixtureDef.density = 1;
    CircleEntity::fixtureDef.friction = 0.3f;
    CircleEntity::fixtureDef.restitution = 0.5f;
}
CircleEntity::~CircleEntity()
{
    delete[] CircleEntity::pathToTexture;

    CircleEntity::body->GetWorld()->DestroyBody(CircleEntity::body);
}

void CircleEntity::Reset()
{
    CircleEntity::body->SetTransform(b2Vec2(CircleEntity::x, CircleEntity::y), 0);
    CircleEntity::body->SetLinearVelocity(CircleEntity::vel);
}
void CircleEntity::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom)
{
    b2Vec2 pos = CircleEntity::body->GetPosition();
    filledCircleColor(renderer, (pos.x * zoom) + x_offset, (pos.y * zoom) + y_offset, CircleEntity::radius * zoom, CircleEntity::color);
}
