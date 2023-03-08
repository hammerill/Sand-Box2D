#include "PObjPlatform.h"

PObjPlatform::PObjPlatform(PlatformDesc platformDesc)
{
    PObjPlatform::platformDesc = platformDesc;

    b2Vec2 start_point = b2Vec2(PObjPlatform::platformDesc.x1, PObjPlatform::platformDesc.y1);
    b2Vec2 end_point = b2Vec2(PObjPlatform::platformDesc.x2, PObjPlatform::platformDesc.y2);
    
    PObjPlatform::platformShape.SetTwoSided(start_point, end_point);

    PObjPlatform::fixtureDef.shape = &(PObjPlatform::platformShape);
}
PObjPlatform::~PObjPlatform()
{
    PObjPlatform::body->GetWorld()->DestroyBody(PObjPlatform::body);
}

void PObjPlatform::SetParam(std::string name, Json::Value value)
{
    if (name == "id")
        PObjPlatform::id = value.asInt();

    else if (name == "r")
        PObjPlatform::r = value.asUInt();
    else if (name == "g")
        PObjPlatform::g = value.asUInt();
    else if (name == "b")
        PObjPlatform::b = value.asUInt();
        
    else if (name == "undeletable")
        PObjPlatform::undeletable = value.asBool();
}

Json::Value PObjPlatform::GetParam(std::string name)
{
    if (name == "id")
        return Json::Value(PObjPlatform::id);
        
    else if (name == "r")
        return Json::Value(PObjPlatform::r);
    else if (name == "g")
        return Json::Value(PObjPlatform::g);
    else if (name == "b")
        return Json::Value(PObjPlatform::b);
        
    else if (name == "undeletable")
        return Json::Value(PObjPlatform::undeletable);

    return 0;
}

void PObjPlatform::Register(b2World* world, Renderer* rr, std::map<std::string, SDL_Texture*>& textures)
{
    PObjPlatform::body = world->CreateBody(&(PObjPlatform::bodyDef));
    PObjPlatform::body->CreateFixture(&(PObjPlatform::fixtureDef));

    PObjPlatform::isRegistered = true;
}

bool PObjPlatform::Render(Renderer* rr, float x_offset, float y_offset, float zoom)
{
    float x1 = (PObjPlatform::platformDesc.x1 * zoom) + x_offset;
    float y1 = (PObjPlatform::platformDesc.y1 * zoom) + y_offset;
    float x2 = (PObjPlatform::platformDesc.x2 * zoom) + x_offset;
    float y2 = (PObjPlatform::platformDesc.y2 * zoom) + y_offset;

    SDL_FRect screen = {
        0, 0,
        (float)(rr->GetWidth()),
        (float)(rr->GetHeight())
    };

    if (!SDL_IntersectFRectAndLine(&screen, &x1, &y1, &x2, &y2))
        return false;

    SDL_SetRenderDrawColor(rr->GetRenderer(), PObjPlatform::r, PObjPlatform::g, PObjPlatform::b, 0xFF);
    SDL_RenderDrawLine(rr->GetRenderer(), x1, y1, x2, y2);

    return true;
}

float PObjPlatform::GetX() 
{
    return (PObjPlatform::platformDesc.x1 + PObjPlatform::platformDesc.x2) / 2;
}
float PObjPlatform::GetY() 
{
    return (PObjPlatform::platformDesc.y1 + PObjPlatform::platformDesc.y2) / 2;
}
