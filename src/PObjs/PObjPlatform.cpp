#include "PObjPlatform.h"

PObjPlatform::PObjPlatform(float x1_plat, float y1_plat, float x2_plat, float y2_plat, uint8_t r, uint8_t g, uint8_t b)
{
    PObjPlatform::x1 = x1_plat;
    PObjPlatform::y1 = y1_plat;
    PObjPlatform::x2 = x2_plat;
    PObjPlatform::y2 = y2_plat;

    PObjPlatform::r = r;
    PObjPlatform::g = g;
    PObjPlatform::b = b;

    b2Vec2 start_point = b2Vec2(PObjPlatform::x1, PObjPlatform::y1);
    b2Vec2 end_point = b2Vec2(PObjPlatform::x2, PObjPlatform::y2);
    
    PObjPlatform::platformShape.SetTwoSided(start_point, end_point);

    PObjPlatform::fixtureDef.shape = &(PObjPlatform::platformShape);
}
PObjPlatform::~PObjPlatform()
{
    delete[] PObjPlatform::pathToTexture;

    PObjPlatform::body->GetWorld()->DestroyBody(PObjPlatform::body);
}

bool PObjPlatform::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height)
{
    b2Vec2 begin =  {(PObjPlatform::x1 * zoom) + x_offset, (PObjPlatform::y1 * zoom) + y_offset};
    b2Vec2 end =    {(PObjPlatform::x2 * zoom) + x_offset, (PObjPlatform::y2 * zoom) + y_offset};

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
