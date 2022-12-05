#include "PObjBox.h"

PObjBox::PObjBox(BoxDesc boxDesc)
{
    PObjBox::boxDesc = boxDesc;

    PObjBox::vel.Set(PObjBox::boxDesc.vel_x, PObjBox::boxDesc.vel_y);
    PObjBox::vel_ang = boxDesc.vel_ang;

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

void PObjBox::SetParam(std::string name, Json::Value value)
{
    if (name == "id")
        PObjBox::id = value.asInt();
    
    else if (name == "x")
    {
        b2Vec2 pos = PObjBox::body->GetPosition();
        pos.x = value.asFloat();
        PObjBox::body->SetTransform(pos, PObjBox::body->GetAngle());
    }
    else if (name == "y")
    {
        b2Vec2 pos = PObjBox::body->GetPosition();
        pos.y = value.asFloat();
        PObjBox::body->SetTransform(pos, PObjBox::body->GetAngle());
    }
    else if (name == "angle")
    {
        b2Vec2 pos = PObjBox::body->GetPosition();
        PObjBox::body->SetTransform(pos, value.asFloat() / PObjBox::RAD2DEG);
    }
    else if (name == "vel_x")
    {
        b2Vec2 vel = PObjBox::body->GetLinearVelocity();
        vel.x = value.asFloat();
        PObjBox::body->SetLinearVelocity(vel);
    }
    else if (name == "vel_y")
    {
        b2Vec2 vel = PObjBox::body->GetLinearVelocity();
        vel.y = value.asFloat();
        PObjBox::body->SetLinearVelocity(vel);
    }
    else if (name == "vel_ang")
    {
        PObjBox::body->SetAngularVelocity(value.asFloat());
    }
}

Json::Value PObjBox::GetParam(std::string name)
{
    if (name == "id")
        return Json::Value(PObjBox::id);
    
    else if (name == "x")
        return Json::Value(PObjBox::body->GetPosition().x);
    else if (name == "y")
        return Json::Value(PObjBox::body->GetPosition().y);
    else if (name == "w")
        return Json::Value(PObjBox::boxDesc.w);
    else if (name == "h")
        return Json::Value(PObjBox::boxDesc.h);
    else if (name == "angle")
        return Json::Value(PObjBox::body->GetAngle() * PObjBox::RAD2DEG);
    else if (name == "vel_x")
        return Json::Value(PObjBox::body->GetLinearVelocity().x);
    else if (name == "vel_y")
        return Json::Value(PObjBox::body->GetLinearVelocity().y);
    else if (name == "vel_ang")
        return Json::Value(PObjBox::body->GetAngularVelocity());
    
    return 0;
}

void PObjBox::SetTexture(SDL_Texture* texture)
{
    PObjBox::texture = texture;
}

void PObjBox::Register(b2World* world, SDL_Renderer* renderer)
{
    PObjBox::body = world->CreateBody(&(PObjBox::bodyDef));
    PObjBox::body->SetLinearVelocity(PObjBox::vel);
    PObjBox::body->SetAngularVelocity(PObjBox::vel_ang);
    PObjBox::body->CreateFixture(&(PObjBox::fixtureDef));
}

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
