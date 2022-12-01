#include "PObjCircle.h"

PObjCircle::PObjCircle(CircleDesc circleDesc)
{
    PObjCircle::circleDesc = circleDesc;

    PObjCircle::vel.Set(PObjCircle::circleDesc.vel_x, PObjCircle::circleDesc.vel_y);

    PObjCircle::bodyDef.type = b2_dynamicBody;
    PObjCircle::bodyDef.position.Set(PObjCircle::circleDesc.x, PObjCircle::circleDesc.y);
    PObjCircle::bodyDef.angle = PObjCircle::circleDesc.angle / PObjCircle::RAD2DEG;

    PObjCircle::circleShape.m_radius = PObjCircle::circleDesc.radius;

    PObjCircle::fixtureDef.shape = &(PObjCircle::circleShape);
    PObjCircle::fixtureDef.density = 1;
    PObjCircle::fixtureDef.friction = 0.3f;
    PObjCircle::fixtureDef.restitution = 0.5f;
}
PObjCircle::~PObjCircle()
{
    PObjCircle::body->GetWorld()->DestroyBody(PObjCircle::body);
}

template<typename T>
void PObjCircle::SetParam(std::string name, T value)
{
    if constexpr(std::is_same<T, float>::value)
    {
        if (name == "x")
        {
            b2Vec2 pos = PObjCircle::body->GetPosition();
            pos.x = (float)value;
            PObjCircle::body->SetTransform(pos, PObjCircle::body->GetAngle());
        }
        else if (name == "y")
        {
            b2Vec2 pos = PObjCircle::body->GetPosition();
            pos.y = (float)value;
            PObjCircle::body->SetTransform(pos, PObjCircle::body->GetAngle());
        }
        else if (name == "angle")
        {
            b2Vec2 pos = PObjCircle::body->GetPosition();
            PObjCircle::body->SetTransform(pos, (float)value / PObjCircle::RAD2DEG);
        }
        else if (name == "vel_x")
        {
            b2Vec2 vel = PObjCircle::body->GetLinearVelocity();
            vel.x = (float)value;
            PObjCircle::body->SetLinearVelocity(vel);
        }
        else if (name == "vel_y")
        {
            b2Vec2 vel = PObjCircle::body->GetLinearVelocity();
            vel.y = (float)value;
            PObjCircle::body->SetLinearVelocity(vel);
        }
    }
    else if constexpr(std::is_same<T, uint8_t>::value)
    {
        if (name == "r")
            PObjCircle::r = (uint8_t)value;
        else if (name == "g")
            PObjCircle::g = (uint8_t)value;
        else if (name == "b")
            PObjCircle::b = (uint8_t)value;
        else if (name == "r_angle")
            PObjCircle::r_angle = (uint8_t)value;
        else if (name == "g_angle")
            PObjCircle::g_angle = (uint8_t)value;
        else if (name == "b_angle")
            PObjCircle::b_angle = (uint8_t)value;
    }
}
template void PObjCircle::SetParam<float>(std::string name, float value);
template void PObjCircle::SetParam<uint8_t>(std::string name, uint8_t value);

template<typename T>
T PObjCircle::GetParam(std::string name)
{
    if constexpr(std::is_same<T, float>::value)
    {
        if (name == "x")
            return PObjCircle::body->GetPosition().x;
        else if (name == "y")
            return PObjCircle::body->GetPosition().y;
        else if (name == "radius")
            return PObjCircle::circleDesc.radius;
        else if (name == "angle")
            return PObjCircle::body->GetAngle() * PObjCircle::RAD2DEG;
        else if (name == "vel_x")
            return PObjCircle::body->GetLinearVelocity().x;
        else if (name == "vel_y")
            return PObjCircle::body->GetLinearVelocity().y;
        return 0;
    }
    else if constexpr(std::is_same<T, uint8_t>::value)
    {
        if (name == "r")
            return PObjCircle::r;
        else if (name == "g")
            return PObjCircle::g;
        else if (name == "b")
            return PObjCircle::b;
        else if (name == "r_angle")
            return PObjCircle::r_angle;
        else if (name == "g_angle")
            return PObjCircle::g_angle;
        else if (name == "b_angle")
            return PObjCircle::b_angle;
        return 0;
    }
}
template float PObjCircle::GetParam<float>(std::string name);
template uint8_t PObjCircle::GetParam<uint8_t>(std::string name);

bool PObjCircle::Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height)
{
    b2Vec2 pos = PObjCircle::body->GetPosition();

    b2Vec2 circle = {
        (pos.x * zoom) + x_offset,
        (pos.y * zoom) + y_offset
    };
    float radiusZoomed = PObjCircle::circleDesc.radius * zoom;

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
