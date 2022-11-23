#include "Level.h"

float Level::GetRandomFloat(float min, float max)
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

            return Level::GetRandomFloat(min, max);
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

// Gonna delete this hardcoded stuff later.
SDL_Texture* texture;
BasePObj* Level::ParseJsonPObj(Json::Value jsonObj)
{
    try
    {
        if (jsonObj["type"] == "platform")
        {
            return new PObjPlatform(Level::LoadNumber(jsonObj["x1"]),
                                    Level::LoadNumber(jsonObj["y1"]),
                                    Level::LoadNumber(jsonObj["x2"]),
                                    Level::LoadNumber(jsonObj["y2"]),

                                    Level::LoadNumber(jsonObj["r"]),
                                    Level::LoadNumber(jsonObj["g"]),
                                    Level::LoadNumber(jsonObj["b"]) );
        }
        else if (jsonObj["type"] == "box")
        {
            return new PObjBox( texture,
                                Level::LoadNumber(jsonObj["x"]),
                                Level::LoadNumber(jsonObj["y"]),
                                Level::LoadNumber(jsonObj["w"]),
                                Level::LoadNumber(jsonObj["h"]),

                                Level::LoadNumber(jsonObj["angle"]),

                                Level::LoadNumber(jsonObj["vel_x"]),
                                Level::LoadNumber(jsonObj["vel_x"]) );
        }
        else if (jsonObj["type"] == "circle")
        {
            return new PObjCircle(  Level::LoadNumber(jsonObj["x"]),
                                    Level::LoadNumber(jsonObj["y"]),
                                    Level::LoadNumber(jsonObj["radius"]),
                                    
                                    Level::LoadNumber(jsonObj["vel_x"]),
                                    Level::LoadNumber(jsonObj["vel_y"]),
                                    
                                    Level::LoadNumber(jsonObj["r"]),
                                    Level::LoadNumber(jsonObj["g"]),
                                    Level::LoadNumber(jsonObj["b"]),
                                    
                                    Level::LoadNumber(jsonObj["r_angle"]),
                                    Level::LoadNumber(jsonObj["g_angle"]),
                                    Level::LoadNumber(jsonObj["b_angle"]) );
        }
        else
            return nullptr;
    }
    catch(const std::exception& e)
    {
        return nullptr;
    }    
}

Level::~Level()
{
    
}

bool Level::LoadFile(std::string base, std::string filepath, SDL_Renderer* temp_rr)
{
    texture = SDL_CreateTextureFromSurface(temp_rr, IMG_Load("assets/img/box.png"));

    Level::~Level();

    try
    {
        std::ifstream ifs((base + "/" + filepath).c_str());

        Json::Reader reader;
        reader.parse(ifs, Level::jsonLevel);

        ifs.close();

        return true;
    }
    catch(const std::exception& e)
    {
        return false;
    }
}

JsonOptions Level::GetOptions()
{
    return JsonOptions(); //lol
}

JsonCamera Level::GetCamera()
{
    auto camera = JsonCamera();

    camera.type = Level::jsonLevel["camera"]["type"].asString();

    if (camera.type != "attached")
    {
        camera.x = Level::LoadNumber(Level::jsonLevel["camera"]["x"]);
        camera.y = Level::LoadNumber(Level::jsonLevel["camera"]["y"]);    
        camera.move = Level::jsonLevel["camera"]["move"].asBool();
    }

    camera.height = Level::LoadNumber(Level::jsonLevel["camera"]["height"]);
    camera.zoom = Level::jsonLevel["camera"]["zoom"].asBool();

    return camera;
}

std::vector<BasePObj*> Level::GetPObjects()
{
    auto objects = std::vector<BasePObj*>();

    Json::Value jsonObjects = Level::jsonLevel["objects"];
    for (unsigned int i = 0; i < jsonObjects.size(); i++)
    {
        objects.push_back(Level::ParseJsonPObj(jsonObjects[i]));
    }
    
    return objects;
}

std::vector<JsonCycle> Level::GetCycles()
{
    auto cycles = std::vector<JsonCycle>();

    try
    {
        Json::Value jsonCycles = Level::jsonLevel["cycles"];
        for (unsigned int i = 0; i < jsonCycles.size(); i++)
        {
            auto cycle = JsonCycle();

            cycle.delay = Level::LoadNumber(jsonCycles[i]["delay"]);

            auto objects = std::vector<BasePObj*>();
            Json::Value jsonCycleObjects = jsonCycles[i]["objects"];
            for (unsigned int j = 0; j < jsonCycleObjects.size(); j++)
            {
                objects.push_back(Level::ParseJsonPObj(jsonCycleObjects[j]));
            }
            cycle.objects = objects;

            cycles.push_back(cycle);
        }
    }
    catch(const std::exception& e)
    {
        return std::vector<JsonCycle>();
    }
        
    return cycles;
}
