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

void PObjCircle::SetParam(std::string name, Json::Value value)
{
    if (name == "id")
        PObjCircle::id = value.asInt();

    else if (name == "x")
    {
        b2Vec2 pos = PObjCircle::body->GetPosition();
        pos.x = value.asFloat();
        PObjCircle::body->SetTransform(pos, PObjCircle::body->GetAngle());
    }
    else if (name == "y")
    {
        b2Vec2 pos = PObjCircle::body->GetPosition();
        pos.y = value.asFloat();
        PObjCircle::body->SetTransform(pos, PObjCircle::body->GetAngle());
    }
    else if (name == "angle")
    {
        b2Vec2 pos = PObjCircle::body->GetPosition();
        PObjCircle::body->SetTransform(pos, value.asFloat() / PObjCircle::RAD2DEG);
    }
    else if (name == "vel_x")
    {
        b2Vec2 vel = PObjCircle::body->GetLinearVelocity();
        vel.x = value.asFloat();
        PObjCircle::body->SetLinearVelocity(vel);
    }
    else if (name == "vel_y")
    {
        b2Vec2 vel = PObjCircle::body->GetLinearVelocity();
        vel.y = value.asFloat();
        PObjCircle::body->SetLinearVelocity(vel);
    }

    else if (name == "r")
        PObjCircle::r = value.asUInt();
    else if (name == "g")
        PObjCircle::g = value.asUInt();
    else if (name == "b")
        PObjCircle::b = value.asUInt();
    else if (name == "r_angle")
        PObjCircle::r_angle = value.asUInt();
    else if (name == "g_angle")
        PObjCircle::g_angle = value.asUInt();
    else if (name == "b_angle")
        PObjCircle::b_angle = value.asUInt();
}

Json::Value PObjCircle::GetParam(std::string name)
{
    if (name == "id")
        return Json::Value(PObjCircle::id);
    
    else if (name == "x")
        return Json::Value(PObjCircle::body->GetPosition().x);
    else if (name == "y")
        return Json::Value(PObjCircle::body->GetPosition().y);
    else if (name == "radius")
        return Json::Value(PObjCircle::circleDesc.radius);
    else if (name == "angle")
        return Json::Value(PObjCircle::body->GetAngle() * PObjCircle::RAD2DEG);
    else if (name == "vel_x")
        return Json::Value(PObjCircle::body->GetLinearVelocity().x);
    else if (name == "vel_y")
        return Json::Value(PObjCircle::body->GetLinearVelocity().y);
    
    else if (name == "r")
        return Json::Value(PObjCircle::r);
    else if (name == "g")
        return Json::Value(PObjCircle::g);
    else if (name == "b")
        return Json::Value(PObjCircle::b);
    else if (name == "r_angle")
        return Json::Value(PObjCircle::r_angle);
    else if (name == "g_angle")
        return Json::Value(PObjCircle::g_angle);
    else if (name == "b_angle")
        return Json::Value(PObjCircle::b_angle);

    return 0;
}

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
