#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "../PObjs/BasePObj.h"
#include "../PObjs/PObjPlatform.h"
#include "../PObjs/PObjBox.h"
#include "../PObjs/PObjCircle.h"

#include "Json.h"

// Represents level options from JSON as struct.
struct JsonOptions
{
    uint8_t bg_r, bg_g, bg_b;
    b2Vec2 gravity;
    float border_width, border_height;
};

// Represents level camera type as enum.
enum JsonCameraType
{
    CAMERA_TYPE_STATIC = 0,         // Camera will stay static, it can be moved and zoomed by user if autorised.
    CAMERA_TYPE_ATTACHED            // Camera will spectate some object with a certain ID.
};

// Represents level camera options from JSON as struct.
struct JsonCamera
{
    JsonCameraType type;
    bool zoom, move;
    float x, y, height, attached_remain;
    int attached_id;
};

// Represents list of the levels to be generated continuosly as struct and vector within.
struct JsonCycle
{
    int delay;
    std::vector<BasePObj*> objects = std::vector<BasePObj*>();
};

/// @brief Class that represents Sand-Box2D level loaded from JSON.
/// For further info refer to the JSON level file documentation:
/// https://github.com/Hammerill/Sand-Box2D-levels/tree/main/docs
class Level
{
private:
    float GetRandomFloat(float min, float max); // TODO: Move to the tools module.

    /// @brief Load number (parse it), further info here:
    /// https://github.com/Hammerill/Sand-Box2D-levels/blob/main/docs/README-level-file.md#numbers-input
    /// @param input JSON value to be parsed. It can be number itself or string random ("-100:20.5").
    /// @return !!! 0 if something went wrong, otherwise number represented as float. 
    float LoadNumber(Json::Value input);

    /// @brief Same as LoadNumber(), but it returns Json::Value.
    Json::Value LoadNumAsJson(Json::Value input);

    /// @brief JSON value of loaded level. Getters of this class are parsing its contents.
    Json::Value jsonLevel;

    /// @brief Where is Level directory?
    std::string base;

    /// @brief Parse JSON PObject and return it as PObj.
    /// @param jsonObj PObj from JSON to be parsed.
    /// @return nullptr if there was problem parsing object.
    /// Otherwise pointer to the BasePObj realization.
    BasePObj* ParseJsonPObj(Json::Value jsonObj);
public:
    ~Level();

    /// @brief Load JSON file. To access its fields call getters.
    /// @param base where is base data directory located?
    /// @param filepath where is the level file (*.json) located relatively to the "base"?
    /// Will load this file.
    /// @return false if something went wrong, true if everything's ok.
    bool LoadFile(std::string base, std::string filepath);

    // Get level options from JSON as struct.
    JsonOptions GetOptions();
    // Get level camera options from JSON as struct.
    JsonCamera GetCamera();

    /// @brief Get array of PObjects.
    /// @return array of pointers to objects representing childs of BasePObj
    /// (not the BasePObj itself because it's an abstract class).
    std::vector<BasePObj*> GetPObjects();

    /// @brief Get Json Cycle represented as JsonCycle object.
    /// @param index index of the desired cycle.
    JsonCycle GetCycle(int index);

    // How much cycles (object spawners) does this level have?
    size_t GetCyclesCount();

    /// @brief Get JSON of the actions list. Its contents has to be
    /// parsed outside and PerformAction() method called.
    /// @return actions list as Json value.
    Json::Value GetActions();

    /// @brief Get specified PObj by ID and return pointer to it.
    /// @param id find PObj with which ID?
    /// @param pobjs array of pointers to realizations of BasePObj where to find PObj.
    /// @return link to the BasePObj realization (any PObj), nullptr if can't find.
    BasePObj* GetObjectById(int id, std::vector<BasePObj*> pobjs);

    /// @brief Perform specific level action.
    /// @param action what action to perform?
    /// @param pobjs array of pointers to realizations of BasePObj.
    /// Necessary because we need to access PObjs involved in the action.
    void PerformAction(Json::Value action, std::vector<BasePObj*> pobjs);
};
