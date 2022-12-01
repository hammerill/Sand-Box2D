#include "PObjBox.h"

PObjBox::PObjBox(BoxDesc boxDesc)
{
    PObjBox::boxDesc = boxDesc;

    PObjBox::vel.Set(PObjBox::boxDesc.vel_x, PObjBox::boxDesc.vel_y);

    PObjBox::bodyDef.type = b2_dynamicBody;
    PObjBox::bodyDef.angle = PObjBox::boxDesc.angle / PObjBox::RAD2DEG; 
    PObjBox::bodyDef.position.Set(PObjBox::boxDesc.x, PObjBox::boxDesc.y);

    PObjBox::boxShape.SetAsBox(PObjBox::boxDesc.w / 2.0f, PObjBox::boxDesc.h / 2.0f);
    
    PObjBox::fixtureDef.shape = &(PObjBox::boxShape);
    PObjBox::fixtureDef.density = 1;
    PObjBox::fixtureDef.friction = 0.3f;
    PObjBox::fixtureDef.restitution = 0.5f;
}
PObjBox::~PObjBox()
{   
    PObjBox::body->GetWorld()->DestroyBody(PObjBox::body);
}

template<typename T>
void PObjBox::SetParam(std::string name, T value)
{
    if constexpr(std::is_same<T, float>::value)
    {
        if (name == "x")
        {
            b2Vec2 pos = PObjBox::body->GetPosition();
            pos.x = (float)value;
            PObjBox::body->SetTransform(pos, PObjBox::body->GetAngle());
        }
        else if (name == "y")
        {
            b2Vec2 pos = PObjBox::body->GetPosition();
            pos.y = (float)value;
            PObjBox::body->SetTransform(pos, PObjBox::body->GetAngle());
        }
        else if (name == "angle")
        {
            b2Vec2 pos = PObjBox::body->GetPosition();
            PObjBox::body->SetTransform(pos, (float)value / PObjBox::RAD2DEG);
        }
        else if (name == "vel_x")
        {
            b2Vec2 vel = PObjBox::body->GetLinearVelocity();
            vel.x = (float)value;
            PObjBox::body->SetLinearVelocity(vel);
        }
        else if (name == "vel_y")
        {
            b2Vec2 vel = PObjBox::body->GetLinearVelocity();
            vel.y = (float)value;
            PObjBox::body->SetLinearVelocity(vel);
        }
    }
    else if constexpr(std::is_same<T, SDL_Texture*>::value)
    {
        if (name == "texture")
            PObjBox::texture = (SDL_Texture*)value;
    }
}
template void PObjBox::SetParam<float>(std::string name, float value);
template void PObjBox::SetParam<SDL_Texture*>(std::string name, SDL_Texture* value);

template<typename T>
T PObjBox::GetParam(std::string name)
{
    if constexpr(std::is_same<T, float>::value)
    {
        if (name == "x")
            return PObjBox::body->GetPosition().x;
        else if (name == "y")
            return PObjBox::body->GetPosition().y;
        else if (name == "w")
            return PObjBox::boxDesc.w;
        else if (name == "h")
            return PObjBox::boxDesc.h;
        else if (name == "angle")
            return PObjBox::body->GetAngle() * PObjBox::RAD2DEG;
        else if (name == "vel_x")
            return PObjBox::body->GetLinearVelocity().x;
        else if (name == "vel_y")
            return PObjBox::body->GetLinearVelocity().y;
        return 0;
    }
    else if constexpr(std::is_same<T, SDL_Texture*>::value)
    {
        if (name == "texture")
            return PObjBox::texture;
        return nullptr;
    }
}
template float PObjBox::GetParam<float>(std::string name);
template SDL_Texture* PObjBox::GetParam<SDL_Texture*>(std::string name);

bool PObjBox::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height)
{
    b2Vec2 pos = PObjBox::body->GetPosition();
    SDL_Rect box;
    
    box.w = PObjBox::boxDesc.w * zoom;
    box.h = PObjBox::boxDesc.h * zoom;

    box.x = (pos.x * zoom) + x_offset - (box.w / 2.0f);
    box.y = (pos.y * zoom) + y_offset - (box.h / 2.0f);

    float maxima = std::max(box.w * 1.5, box.h * 1.5);

    if (box.x > -maxima && box.x < width + maxima
    &&  box.y > -maxima && box.y < height + maxima)
    {
        SDL_RenderCopyEx(renderer, PObjBox::texture, NULL, &box, PObjBox::body->GetAngle() * PObjBox::RAD2DEG, NULL, SDL_FLIP_NONE);
        return true;
    }
    else
        return false;
}
