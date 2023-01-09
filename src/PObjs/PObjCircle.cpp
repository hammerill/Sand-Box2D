#include "PObjCircle.h"

PObjCircle::PObjCircle(CircleDesc circleDesc)
{
    PObjCircle::circleDesc = circleDesc;

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
    else if (name == "vel_ang")
    {
        PObjCircle::body->SetAngularVelocity(value.asFloat());
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
    
    else if (name == "is_texture")
        PObjCircle::is_texture = value.asBool();
    else if (name == "texture_path")
        PObjCircle::texture_path = value.asString();
    else if (name == "undeletable")
        PObjCircle::undeletable = value.asBool();
}

Json::Value PObjCircle::GetParam(std::string name)
{
    if (name == "id")
        return Json::Value(PObjCircle::id);
    
    if (PObjCircle::isRegistered)
    {
        if (name == "x")
            return Json::Value(PObjCircle::body->GetPosition().x);
        else if (name == "y")
            return Json::Value(PObjCircle::body->GetPosition().y);
        else if (name == "angle")
            return Json::Value(PObjCircle::body->GetAngle() * PObjCircle::RAD2DEG);
        else if (name == "vel_x")
            return Json::Value(PObjCircle::body->GetLinearVelocity().x);
        else if (name == "vel_y")
            return Json::Value(PObjCircle::body->GetLinearVelocity().y);
        else if (name == "vel_ang")
            return Json::Value(PObjCircle::body->GetAngularVelocity());
    }
    else
    {
        if (name == "x")
            return Json::Value(PObjCircle::circleDesc.x);
        else if (name == "y")
            return Json::Value(PObjCircle::circleDesc.y);
        else if (name == "angle")
            return Json::Value(PObjCircle::circleDesc.angle);
        else if (name == "vel_x")
            return Json::Value(PObjCircle::circleDesc.vel_x);
        else if (name == "vel_y")
            return Json::Value(PObjCircle::circleDesc.vel_y);
        else if (name == "vel_ang")
            return Json::Value(PObjCircle::circleDesc.vel_ang);
    }
    
    if (name == "radius")
        return Json::Value(PObjCircle::circleDesc.radius);
    
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

    else if (name == "is_texture")
        return Json::Value(PObjCircle::is_texture);
    else if (name == "texture_path")
        return Json::Value(PObjCircle::texture_path);
    else if (name == "undeletable")
        return Json::Value(PObjCircle::undeletable);

    return 0;
}

void PObjCircle::Register(b2World* world, SDL_Renderer* renderer, std::map<std::string, SDL_Texture*>& textures)
{
    PObjCircle::body = world->CreateBody(&(PObjCircle::bodyDef));
    PObjCircle::body->SetLinearVelocity(b2Vec2(PObjCircle::circleDesc.vel_x, PObjCircle::circleDesc.vel_y));
    PObjCircle::body->SetAngularVelocity(PObjCircle::circleDesc.vel_ang);
    PObjCircle::body->CreateFixture(&(PObjCircle::fixtureDef));

    if (PObjCircle::is_texture)
        PObjCircle::texture = PObjCircle::LoadTexture(textures, PObjCircle::GetParam("texture_path").asString(), renderer);

    PObjCircle::isRegistered = true;
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
        if (PObjCircle::is_texture)
        {
            SDL_Rect box;
    
            box.w = radiusZoomed * 2;
            box.h = radiusZoomed * 2;

            box.x = circle.x - (box.w / 2.0f);
            box.y = circle.y - (box.h / 2.0f);

            SDL_RenderCopyEx(renderer, PObjCircle::texture, NULL, &box, PObjCircle::body->GetAngle() * PObjCircle::RAD2DEG, NULL, SDL_FLIP_NONE);
        }
        else
        {
            filledCircleRGBA(renderer, circle.x, circle.y, radiusZoomed, PObjCircle::r, PObjCircle::g, PObjCircle::b, 0xFF);

            SDL_SetRenderDrawColor(renderer, PObjCircle::r_angle, PObjCircle::g_angle, PObjCircle::b_angle, 0);
            SDL_RenderDrawLine(renderer, 
            circle.x, 
            circle.y, 
            (circle.x) + (cos(PObjCircle::body->GetAngle()) * radiusZoomed), 
            (circle.y) + (sin(PObjCircle::body->GetAngle()) * radiusZoomed));
        }

        return true;
    }
    else
        return false;
}
