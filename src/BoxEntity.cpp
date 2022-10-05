#include "BoxEntity.h"

BoxEntity::BoxEntity(b2World* world, SDL_Renderer* renderer, SDL_Surface* tmp_sprites, float x_box, float y_box, float w_box, float h_box, float angle_box)
{
    BoxEntity::world = world;
    BoxEntity::renderer = renderer;
    BoxEntity::x_box = x_box;
    BoxEntity::y_box = y_box;
    BoxEntity::w_box = w_box;
    BoxEntity::h_box = h_box;
    BoxEntity::angle_box = angle_box;

    SetSprite(tmp_sprites);

    BoxEntity::boxBodyDef.type = b2_dynamicBody;
    BoxEntity::boxBodyDef.angle = BoxEntity::angle_box; 
    BoxEntity::boxBodyDef.position.Set(BoxEntity::x_box, BoxEntity::y_box);

    BoxEntity::vel.Set(0, 0.2f);

    BoxEntity::Body = BoxEntity::world->CreateBody(&boxBodyDef);
    BoxEntity::Body->SetLinearVelocity(BoxEntity::vel);
    
    BoxEntity::dynamicBox.SetAsBox((BoxEntity::w_box / 2.0f) - BoxEntity::dynamicBox.m_radius, (BoxEntity::h_box / 2.0f) - BoxEntity::dynamicBox.m_radius);
    
    BoxEntity::fixtureDef.shape = &(BoxEntity::dynamicBox);
    BoxEntity::fixtureDef.density = 1;
    BoxEntity::fixtureDef.friction = 0.3f;
    BoxEntity::fixtureDef.restitution = 0.5f;
    BoxEntity::Body->CreateFixture(&(BoxEntity::fixtureDef));

    BoxEntity::box.w = BoxEntity::w_box * BoxEntity::MET2PIX;
    BoxEntity::box.h = BoxEntity::h_box * BoxEntity::MET2PIX;
}
BoxEntity::~BoxEntity()
{
    SDL_DestroyTexture(BoxEntity::texture_box);
}

void BoxEntity::SetSprite(SDL_Surface* tmp_sprites)
{
    BoxEntity::texture_box = SDL_CreateTextureFromSurface(BoxEntity::renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
}

void BoxEntity::Reset()
{
    BoxEntity::Body->SetTransform(b2Vec2(BoxEntity::x_box, BoxEntity::y_box), BoxEntity::angle_box);
    BoxEntity::Body->SetLinearVelocity(BoxEntity::vel);
}
void BoxEntity::Step()
{
    b2Vec2 pos = BoxEntity::Body->GetPosition();

    BoxEntity::box.x = pos.x * BoxEntity::MET2PIX - BoxEntity::box.w / 2;
    BoxEntity::box.y = pos.y * BoxEntity::MET2PIX - BoxEntity::box.h / 2;
}
void BoxEntity::Render()
{    
    SDL_RenderCopyEx(BoxEntity::renderer, BoxEntity::texture_box, NULL, &(BoxEntity::box), BoxEntity::Body->GetAngle() * BoxEntity::RAD2DEG, NULL, SDL_FLIP_NONE);
}
