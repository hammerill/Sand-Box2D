#include "PObjBox.h"

PObjBox::PObjBox(const char* path_to_texture, float x_box, float y_box, float w_box, float h_box, float angle_box, float vel_x, float vel_y)
{
    PObjBox::x = x_box;
    PObjBox::y = y_box;
    PObjBox::w = w_box;
    PObjBox::h = h_box;
    PObjBox::angle = angle_box;

    PObjBox::pathToTexture = path_to_texture;
    
    PObjBox::bodyDef.type = b2_dynamicBody;
    PObjBox::bodyDef.angle = PObjBox::angle; 
    PObjBox::bodyDef.position.Set(PObjBox::x, PObjBox::y);

    PObjBox::vel.Set(vel_x, vel_y);

    PObjBox::boxShape.SetAsBox(PObjBox::w / 2.0f, PObjBox::h / 2.0f);
    
    PObjBox::fixtureDef.shape = &(PObjBox::boxShape);
    PObjBox::fixtureDef.density = 1;
    PObjBox::fixtureDef.friction = 0.3f;
    PObjBox::fixtureDef.restitution = 0.5f;
}

PObjBox::PObjBox(SDL_Texture* texture, float x_box, float y_box, float w_box, float h_box, float angle_box, float vel_x, float vel_y)
{
    PObjBox::x = x_box;
    PObjBox::y = y_box;
    PObjBox::w = w_box;
    PObjBox::h = h_box;
    PObjBox::angle = angle_box;

    PObjBox::pathToTexture = nullptr;
    PObjBox::texture = texture;
    
    PObjBox::bodyDef.type = b2_dynamicBody;
    PObjBox::bodyDef.angle = PObjBox::angle; 
    PObjBox::bodyDef.position.Set(PObjBox::x, PObjBox::y);

    PObjBox::vel.Set(vel_x, vel_y);

    PObjBox::boxShape.SetAsBox(PObjBox::w / 2.0f, PObjBox::h / 2.0f);
    
    PObjBox::fixtureDef.shape = &(PObjBox::boxShape);
    PObjBox::fixtureDef.density = 1;
    PObjBox::fixtureDef.friction = 0.3f;
    PObjBox::fixtureDef.restitution = 0.5f;
}
PObjBox::~PObjBox()
{   
    if (PObjBox::pathToTexture != nullptr) 
        SDL_DestroyTexture(PObjBox::texture);

    delete[] PObjBox::pathToTexture;

    PObjBox::body->GetWorld()->DestroyBody(PObjBox::body);
}

void PObjBox::Reset()
{
    PObjBox::body->SetTransform(b2Vec2(PObjBox::x, PObjBox::y), PObjBox::angle);
    PObjBox::body->SetLinearVelocity(PObjBox::vel);
}
bool PObjBox::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height)
{
    b2Vec2 pos = PObjBox::body->GetPosition();
    SDL_Rect box;
    
    box.w = PObjBox::w * zoom;
    box.h = PObjBox::h * zoom;

    box.x = (pos.x * zoom) + x_offset - (box.w / 2.0f);
    box.y = (pos.y * zoom) + y_offset - (box.h / 2.0f);

    if (box.x > -(box.w * 2) && box.x < width + box.w
    &&  box.y > -(box.h * 2) && box.y < height + box.h)
    {
        SDL_RenderCopyEx(renderer, PObjBox::texture, NULL, &box, PObjBox::body->GetAngle() * PObjBox::RAD2DEG, NULL, SDL_FLIP_NONE);
        return true;
    }
    else
        return false;
}
