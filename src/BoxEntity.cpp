#include "BoxEntity.h"

BoxEntity::BoxEntity(b2World* world, SDL_Renderer* renderer, const char* path_to_texture, float x_box, float y_box, float w_box, float h_box, float angle_box)
{
    BoxEntity::world = world;
    BoxEntity::x = x_box;
    BoxEntity::y = y_box;
    BoxEntity::w = w_box;
    BoxEntity::h = h_box;
    BoxEntity::angle = angle_box;

    BoxEntity::SetSprite(renderer, path_to_texture);

    BoxEntity::bodyDef.type = b2_dynamicBody;
    BoxEntity::bodyDef.angle = BoxEntity::angle; 
    BoxEntity::bodyDef.position.Set(BoxEntity::x, BoxEntity::y);

    BoxEntity::vel.Set(0, 0.2f);

    BoxEntity::body = BoxEntity::world->CreateBody(&bodyDef);
    BoxEntity::body->SetLinearVelocity(BoxEntity::vel);
    
    BoxEntity::boxShape.SetAsBox(BoxEntity::w / 2.0f, BoxEntity::h / 2.0f);
    
    BoxEntity::fixtureDef.shape = &(BoxEntity::boxShape);
    BoxEntity::fixtureDef.density = 1;
    BoxEntity::fixtureDef.friction = 0.3f;
    BoxEntity::fixtureDef.restitution = 0.5f;
    BoxEntity::body->CreateFixture(&(BoxEntity::fixtureDef));
}

void BoxEntity::Reset()
{
    BoxEntity::body->SetTransform(b2Vec2(BoxEntity::x, BoxEntity::y), BoxEntity::angle);
    BoxEntity::body->SetLinearVelocity(BoxEntity::vel);
}
void BoxEntity::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom)
{
    b2Vec2 pos = BoxEntity::body->GetPosition();

    BoxEntity::box.x = (pos.x * zoom) + x_offset;
    BoxEntity::box.y = (pos.y * zoom) + y_offset;
    
    BoxEntity::box.w = BoxEntity::w * zoom;
    BoxEntity::box.h = BoxEntity::h * zoom;

    SDL_RenderCopyEx(renderer, BoxEntity::texture, NULL, &(BoxEntity::box), BoxEntity::body->GetAngle() * BoxEntity::RAD2DEG, NULL, SDL_FLIP_NONE);
}
