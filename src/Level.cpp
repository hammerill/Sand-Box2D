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
        if (input.asString().find_last_of(":") != std::string::npos)
        {
            float min = std::stof(input.asString().substr(0, input.asString().find_last_of(":")));
            float max = std::stof(input.asString().substr(input.asString().find_last_of(":") + 1));

            return Level::getRandomFloat(min, max);
        }
        else 
        {
            return input.asFloat();
        }
    }
    catch(const std::exception& e)
    {
        return 0;
    }
}

void Level::ProceedPObj(Json::Value jsonObj, std::vector<JsonPObj*>* arrayPObjs)
{
    std::string objType = jsonObj["type"].asString();

    // Yes. It's if/else if/else if/else if/...
    // Yes. It's not a switch.
    if (objType == "platform")
    {
        JsonPObjPlatform* platform = new JsonPObjPlatform();
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
        JsonPObjBox* box = new JsonPObjBox();
        box->type = objType;

        box->x = Level::LoadNumber(jsonObj["x"]);
        box->y = Level::LoadNumber(jsonObj["y"]);
        box->w = Level::LoadNumber(jsonObj["w"]);
        box->h = Level::LoadNumber(jsonObj["h"]);

        box->angle = Level::LoadNumber(jsonObj["angle"]);

        box->vel_x = Level::LoadNumber(jsonObj["vel_x"]);
        box->vel_y = Level::LoadNumber(jsonObj["vel_y"]);

        box->texture = jsonObj["texture"].asString();

        arrayPObjs->push_back(box);
    }
    else if (objType == "circle")
    {
        JsonPObjCircle* circle = new JsonPObjCircle();
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

        arrayPObjs->push_back(circle);
    }
}

Level::~Level()
{
    std::vector<JsonPObj*>().swap(objects);
    std::vector<JsonCycle>().swap(cycles);
}

bool Level::LoadFile(std::string base, std::string filepath)
{
    Level::~Level();
    Level::camera = JsonCamera();

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
        Level::camera.type = jsonLevel["camera"]["type"].asString();

        if (Level::camera.type != "attached")
        {
            Level::camera.x = Level::LoadNumber(jsonLevel["camera"]["x"]);
            Level::camera.y = Level::LoadNumber(jsonLevel["camera"]["y"]);
            Level::camera.move = jsonLevel["camera"]["move"].asBool();
        }

        Level::camera.zoom = jsonLevel["camera"]["zoom"].asBool();
        Level::camera.height    = Level::LoadNumber(jsonLevel["camera"]["height"]);
        /////////

        // OBJECTS
        Json::Value jsonObjects = jsonLevel["objects"];
        for (unsigned int i = 0; i < jsonObjects.size(); i++)
        {
            Level::ProceedPObj(jsonObjects[i], &(Level::objects));
        }        
        //////////

        // CYCLES
        Json::Value jsonCycles = jsonLevel["cycles"];
        for (unsigned int i = 0; i < jsonCycles.size(); i++)
        {
            JsonCycle jsonCycle =
            {
                jsonCycles[i]["delay"],
                jsonCycles[i]["objects"]
            };

            Level::cycles.push_back(jsonCycle);
        }        
        /////////

        ifs.close();

        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

std::vector<LoadedCycle> Level::getCycles()
{
    std::vector<LoadedCycle> result = std::vector<LoadedCycle>();

    try
    {
        for (unsigned int i = 0; i < Level::cycles.size(); i++)
        {
            LoadedCycle cycle;

            cycle.delay = Level::LoadNumber(Level::cycles[i].delay);
            for (unsigned int j = 0; j < Level::cycles[i].objects.size(); j++)
            {
                Level::ProceedPObj(Level::cycles[i].objects[j], &(cycle.objects));
            }
            
            result.push_back(cycle);
        }
        
        return result;
    }
    catch(const std::exception& e)
    {
        return std::vector<LoadedCycle>();
    }
}
