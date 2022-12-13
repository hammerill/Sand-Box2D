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
Json::Value Level::LoadNumAsJson(Json::Value input)
{
    return Json::Value(Level::LoadNumber(input));
}

BasePObj* Level::ParseJsonPObj(Json::Value jsonObj)
{
    try
    {
        if (jsonObj["type"] == "platform")
        {
            PlatformDesc platformDesc =
            {
                Level::LoadNumber(jsonObj["x1"]),
                Level::LoadNumber(jsonObj["y1"]),
                Level::LoadNumber(jsonObj["x2"]),
                Level::LoadNumber(jsonObj["y2"])
            };
            auto platform = new PObjPlatform(platformDesc);

            platform->SetParam("r", Level::LoadNumAsJson(jsonObj["r"]));
            platform->SetParam("g", Level::LoadNumAsJson(jsonObj["g"]));
            platform->SetParam("b", Level::LoadNumAsJson(jsonObj["b"]));

            if (jsonObj.isMember("id"))
                platform->SetParam("id", Level::LoadNumAsJson(jsonObj["id"]));

            return platform;
        }
        else if (jsonObj["type"] == "box")
        {
            BoxDesc boxDesc = 
            {
                Level::LoadNumber(jsonObj["x"]), Level::LoadNumber(jsonObj["y"]),
                Level::LoadNumber(jsonObj["w"]), Level::LoadNumber(jsonObj["h"]),
                Level::LoadNumber(jsonObj["angle"]),
                Level::LoadNumber(jsonObj["vel_x"]), Level::LoadNumber(jsonObj["vel_y"]),
                Level::LoadNumber(jsonObj["vel_ang"])
            };
            auto box = new PObjBox(boxDesc);

            box->SetParam("texture_path", Json::Value(Level::base + "/" + jsonObj["texture"].asString()));

            if (jsonObj.isMember("id"))
                box->SetParam("id", Level::LoadNumAsJson(jsonObj["id"]));

            return box;
        }
        else if (jsonObj["type"] == "circle")
        {
            CircleDesc circleDesc = 
            {
                Level::LoadNumber(jsonObj["x"]), Level::LoadNumber(jsonObj["y"]),
                Level::LoadNumber(jsonObj["radius"]),
                Level::LoadNumber(jsonObj["angle"]),
                Level::LoadNumber(jsonObj["vel_x"]), Level::LoadNumber(jsonObj["vel_y"]),
                Level::LoadNumber(jsonObj["vel_ang"])
            };
            auto circle = new PObjCircle(circleDesc);

            auto texture_path = jsonObj["texture"].asString();

            if (texture_path != "")
            {
                circle->SetParam("is_texture", Json::Value(true));
                circle->SetParam("texture_path", Json::Value(Level::base + "/" + texture_path));
            }
            else
            {
                circle->SetParam("r", Level::LoadNumAsJson(jsonObj["r"]));
                circle->SetParam("g", Level::LoadNumAsJson(jsonObj["g"]));
                circle->SetParam("b", Level::LoadNumAsJson(jsonObj["b"]));
                circle->SetParam("r_angle", Level::LoadNumAsJson(jsonObj["r_angle"]));
                circle->SetParam("g_angle", Level::LoadNumAsJson(jsonObj["g_angle"]));
                circle->SetParam("b_angle", Level::LoadNumAsJson(jsonObj["b_angle"]));
            }

            if (jsonObj.isMember("id"))
                circle->SetParam("id", Level::LoadNumAsJson(jsonObj["id"]));

            return circle;
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
    // SDL_DestroyTexture(texture); 
    // I need to think about how to load/unload textures later.
}

bool Level::LoadFile(std::string base, std::string filepath)
{
    Level::~Level();

    Level::base = base;

    try
    {
        std::ifstream ifs((Level::base + "/" + filepath).c_str());

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

Json::Value Level::GetActions()
{
    return Level::jsonLevel["actions"];
}

BasePObj* Level::GetObjectById(int id, std::vector<BasePObj*> pobjs)
{
    for (size_t i = 0; i < pobjs.size(); i++)
    {
        if (pobjs[i]->GetId() == id)
            return pobjs[i];
    }
    return nullptr;
}

void PerformStep(BasePObj* pobj, std::string param, Json::Value value, std::string type = "set")
{
    if (type == "set")
        pobj->SetParam(param, value);
    else if (type == "add")
        pobj->SetParam(param, Json::Value(pobj->GetParam(param).asFloat() + value.asFloat()));
}
void Level::PerformAction(Json::Value action, std::vector<BasePObj*> pobjs)
{
    for (unsigned int i = 0; i < action.size(); i++)
    {
        auto pobj = Level::GetObjectById(action[i]["id"].asInt(), pobjs);
        if (pobj != nullptr)
        {
            PerformStep(pobj,
                        action[i]["param"].asString(),
                        Level::LoadNumAsJson(action[i]["value"]),
                        action[i]["type"].asString());
        }
    }
}
