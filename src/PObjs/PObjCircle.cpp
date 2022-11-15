#include "PObjCircle.h"

PObjCircle::PObjCircle( float x, float y, float radius, float vel_x, float vel_y,
                        uint8_t r, uint8_t g, uint8_t b,
                        uint8_t r_angle, uint8_t g_angle, uint8_t b_angle)
{
    PObjCircle::x = x;
    PObjCircle::y = y;
    PObjCircle::radius = radius;

    PObjCircle::r = r;
    PObjCircle::g = g;
    PObjCircle::b = b;

    PObjCircle::r_angle = r_angle;
    PObjCircle::g_angle = g_angle;
    PObjCircle::b_angle = b_angle;

    PObjCircle::vel.Set(vel_x, vel_y);

    PObjCircle::bodyDef.type = b2_dynamicBody;
    PObjCircle::bodyDef.position.Set(PObjCircle::x, PObjCircle::y);
    PObjCircle::bodyDef.angle = 0;

    PObjCircle::circleShape.m_radius = radius;

    PObjCircle::fixtureDef.shape = &(PObjCircle::circleShape);
    PObjCircle::fixtureDef.density = 1;
    PObjCircle::fixtureDef.friction = 0.3f;
    PObjCircle::fixtureDef.restitution = 0.5f;
}
PObjCircle::~PObjCircle()
{
    delete[] PObjCircle::pathToTexture;

    PObjCircle::body->GetWorld()->DestroyBody(PObjCircle::body);
}

void PObjCircle::Reset()
{
    PObjCircle::body->SetTransform(b2Vec2(PObjCircle::x, PObjCircle::y), 0);
    PObjCircle::body->SetLinearVelocity(PObjCircle::vel);
}
bool PObjCircle::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height)
{
    b2Vec2 pos = PObjCircle::body->GetPosition();

    b2Vec2 circle = {
        (pos.x * zoom) + x_offset,
        (pos.y * zoom) + y_offset
    };
    float radiusZoomed = PObjCircle::radius * zoom;

    if (circle.x > -radiusZoomed && circle.x < width + radiusZoomed
    &&  circle.y > -radiusZoomed && circle.y < height + radiusZoomed)
    {
        filledCircleRGBA(renderer, circle.x, circle.y, radiusZoomed, PObjCircle::r, PObjCircle::g, PObjCircle::b, 0xFF);

        SDL_SetRenderDrawColor(renderer, PObjCircle::r_angle, PObjCircle::g_angle, PObjCircle::b_angle, 0);
        SDL_RenderDrawLine(renderer, 
        circle.x, 
        circle.y, 
        (circle.x) + (cos(PObjCircle::body->GetAngle()) * radiusZoomed), 
        (circle.y) + (sin(PObjCircle::body->GetAngle()) * radiusZoomed));

        return true;
    }
    else
        return false;
}
