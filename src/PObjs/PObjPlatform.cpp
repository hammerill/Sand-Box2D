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

template<typename T>
void PObjPlatform::SetParam(std::string name, T value)
{
    if constexpr(std::is_same<T, uint8_t>::value)
    {
        if (name == "r")
            PObjPlatform::r = (uint8_t)value;
        else if (name == "g")
            PObjPlatform::g = (uint8_t)value;
        else if (name == "b")
            PObjPlatform::b = (uint8_t)value;
    }
}
template void PObjPlatform::SetParam<uint8_t>(std::string name, uint8_t value);

template<typename T>
T PObjPlatform::GetParam(std::string name)
{
    if constexpr(std::is_same<T, uint8_t>::value)
    {
        if (name == "r")
            return PObjPlatform::r;
        else if (name == "g")
            return PObjPlatform::g;
        else if (name == "b")
            return PObjPlatform::b;
        return 0;
    }
}
template uint8_t PObjPlatform::GetParam<uint8_t>(std::string name);

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
