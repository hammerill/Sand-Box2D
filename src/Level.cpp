#include "Level.h"

float Level::getRandomFloat(float min, float max)
{
    return  min
            +
            static_cast<float> (rand())
            /
            (static_cast<float> (RAND_MAX / (max-min)));
}

float Level::LoadNumber(Json::Value input)
{
    try
    {
        if (input.asString().find_last_of("-") != std::string::npos)
        {
            float min = std::stof(input.asString().substr(0, input.asString().find_last_of("-")));
            float max = std::stof(input.asString().substr(input.asString().find_last_of("-") + 1));

            return Level::getRandomFloat(min, max);
        }
        else 
        {
            return input.asFloat();
        }
    }
    catch(const std::exception& e)
    {
        return NULL;
    }    
}

void Level::ProceedPObj(Json::Value jsonObj, std::vector<JsonPObj*>* arrayPObjs)
{
    std::string objType = jsonObj["type"].asString();

    // Yes. It's if/else if/else if/else if/...
    // Yes. It's not a switch.
    if (objType == "platform")
    {
        JsonPObjPlatform* platform;
        platform->type = objType;

        platform->x1 = Level::LoadNumber(jsonObj["x1"]);
        platform->y1 = Level::LoadNumber(jsonObj["y1"]);
        platform->x2 = Level::LoadNumber(jsonObj["x2"]);
        platform->y2 = Level::LoadNumber(jsonObj["y2"]);

        platform->r = Level::LoadNumber(jsonObj["r"]);
        platform->g = Level::LoadNumber(jsonObj["g"]);
        platform->b = Level::LoadNumber(jsonObj["b"]);

        arrayPObjs->push_back(platform);
    }
    else if (objType == "box")
    {
        JsonPObjBox* box;
        box->type = objType;

        box->x = Level::LoadNumber(jsonObj["x"]);
        box->y = Level::LoadNumber(jsonObj["y"]);
        box->w = Level::LoadNumber(jsonObj["w"]);
        box->h = Level::LoadNumber(jsonObj["h"]);

        box->angle = Level::LoadNumber(jsonObj["angle"]);

        box->vel_x = Level::LoadNumber(jsonObj["vel_x"]);
        box->vel_y = Level::LoadNumber(jsonObj["vel_y"]);

        box->texture = jsonObj["texture"].asString();
    }
    else if (objType == "circle")
    {
        JsonPObjCircle* circle;
        circle->type = objType;

        circle->x = Level::LoadNumber(jsonObj["x"]);
        circle->y = Level::LoadNumber(jsonObj["y"]);
        
        circle->radius = Level::LoadNumber(jsonObj["radius"]);

        circle->vel_x = Level::LoadNumber(jsonObj["vel_x"]);
        circle->vel_y = Level::LoadNumber(jsonObj["vel_y"]);

        circle->r = Level::LoadNumber(jsonObj["r"]);
        circle->g = Level::LoadNumber(jsonObj["g"]);
        circle->b = Level::LoadNumber(jsonObj["b"]);

        circle->r_angle = Level::LoadNumber(jsonObj["r_angle"]);
        circle->g_angle = Level::LoadNumber(jsonObj["g_angle"]);
        circle->b_angle = Level::LoadNumber(jsonObj["b_angle"]);
    }
}

Level::~Level()
{
    objects->clear();
    cycles->clear();
}

std::exception* Level::LoadFile(std::string base, std::string filepath)
{
    try
    {
        std::ifstream ifs((base + "/" + filepath).c_str());

        Json::Reader reader;
        Json::Value jsonLevel;

        reader.parse(ifs, jsonLevel);

        // METADATA
        Level::metadata = jsonLevel["metadata"];
        ///////////

        // OPTIONS
        // sorry nothing
        //////////

        // CAMERA
        Level::camera.type = jsonLevel["type"].asString();

        if (Level::camera.type != "attached")
        {
            Level::camera.x = Level::LoadNumber(jsonLevel["x"]);
            Level::camera.y = Level::LoadNumber(jsonLevel["y"]);
            Level::camera.move = jsonLevel["move"].asBool();
        }

        Level::camera.zoom = jsonLevel["zoom"].asBool();
        Level::camera.height    = Level::LoadNumber(jsonLevel["height"]);
        /////////

        // OBJECTS
        const Json::Value& jsonObjects = jsonLevel["objects"];
        for (int i = 0; i < jsonObjects.size(); i++)
        {
            Level::ProceedPObj(jsonObjects[i], Level::objects);
        }
        
        //////////

        return nullptr;
    }
    catch(std::exception* e)
    {
        return e;
    }
    
}
