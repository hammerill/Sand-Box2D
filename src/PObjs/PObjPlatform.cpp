#include "PObjPlatform.h"

PObjPlatform::PObjPlatform(float x1_plat, float y1_plat, float x2_plat, float y2_plat)
{
    PObjPlatform::x1 = x1_plat;
    PObjPlatform::y1 = y1_plat;
    PObjPlatform::x2 = x2_plat;
    PObjPlatform::y2 = y2_plat;

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

void PObjPlatform::Reset() {}
bool PObjPlatform::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height)
{    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

    SDL_RenderDrawLine(renderer, 
    (PObjPlatform::x1 * zoom) + x_offset, 
    (PObjPlatform::y1 * zoom) + y_offset, 
    (PObjPlatform::x2 * zoom) + x_offset, 
    (PObjPlatform::y2 * zoom) + y_offset);

    return true; // It was too damn hard to determine is platform in the screen bounds, so I just render it always and always return true
}
