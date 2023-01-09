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

void PObjPlatform::Register(b2World* world, SDL_Renderer* renderer, std::map<std::string, SDL_Texture*>& textures)
{
    PObjPlatform::body = world->CreateBody(&(PObjPlatform::bodyDef));
    PObjPlatform::body->CreateFixture(&(PObjPlatform::fixtureDef));

    PObjPlatform::isRegistered = true;
}

bool PObjPlatform::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height)
{
    b2Vec2 begin =  {(PObjPlatform::platformDesc.x1 * zoom) + x_offset, (PObjPlatform::platformDesc.y1 * zoom) + y_offset};
    b2Vec2 end =    {(PObjPlatform::platformDesc.x2 * zoom) + x_offset, (PObjPlatform::platformDesc.y2 * zoom) + y_offset};

    // Formula to calculate distance between two points
    float halfDistance =    sqrt(
                                pow(end.x - begin.x, 2)
                                +
                                pow(end.y - begin.y, 2)
                            )
                            /2;

    /// Yes, in order to determine is the platform in the screen bounds, 
    /// I just used the same formula as for circle, i.e. look at this center,
    /// and check its bounding with screen taking care of radius, 
    /// but platform does not have a radius, 
    /// but we can calculate half distance between begin at end instead.
    if ((begin.x + end.x) / 2 > -halfDistance && (begin.x + end.x) / 2 < width + halfDistance 
    &&  (begin.y + end.y) / 2 > -halfDistance && (begin.y + end.y) / 2 < height + halfDistance)
    {
        SDL_SetRenderDrawColor(renderer, PObjPlatform::r, PObjPlatform::g, PObjPlatform::b, 0);
        SDL_RenderDrawLine(renderer, begin.x, begin.y, end.x, end.y);

        return true;
    }
    else
        return false;
}
