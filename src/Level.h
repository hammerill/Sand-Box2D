#pragma once

#include <jsoncpp/json/json.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "PObjs/BasePObj.h"
#include "PObjs/PObjPlatform.h"
#include "PObjs/PObjBox.h"
#include "PObjs/PObjCircle.h"

struct JsonOptions
{
    // Gonna be filled in some time.
};

struct JsonCamera
{
    std::string type;
    bool zoom, move;
    float x, y, height;
};

struct JsonCycle
{
    int delay;
    std::vector<BasePObj*> objects = std::vector<BasePObj*>();
};

/// @brief Class that represents Sand-Box2D level loaded from JSON. 
/// Its fields should be handled by some level loader.
/// For further info refer to the JSON level file documentation:
/// https://github.com/Hammerill/Sand-Box2D-levels/tree/main/docs
class Level
{
private:
    float GetRandomFloat(float min, float max);

    /// @brief Load number (parse it), further info here:
    /// https://github.com/Hammerill/Sand-Box2D-levels/blob/main/docs/README-level-file.md#numbers-input
    /// @param input JSON value to be parsed. It can be number itself or string random ("-100:20.5").
    /// @return !!! 0 if something went wrong, otherwise number represented as float. 
    float LoadNumber(Json::Value input);

    /// @brief Same as LoadNumber(), but it returns Json::Value.
    Json::Value LoadNumAsJson(Json::Value input);

    /// @brief JSON value of loaded level. Getters of this class are parsing its contents.
    Json::Value jsonLevel;

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
 
    JsonOptions GetOptions();
 
    JsonCamera GetCamera();
 
    /// @brief Get array of PObjects.
    /// @return array of pointers to objects represeneting childs of BasePObj
    /// (not the BasePObj itself because it's an abstract class).
    std::vector<BasePObj*> GetPObjects();

    /// @brief Get array of cycles from JSON represented as structs.
    /// @return empty vector if something went wrong, otherwise filled JsonCycle vector
    /// (which contains delay info and PObjects array).
    std::vector<JsonCycle> GetCycles();

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
