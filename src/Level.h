#pragma once

#include <json/json.h>

#include <iostream>
#include <fstream>
#include <vector>

struct JsonOptions
{

};

struct JsonCamera
{
    std::string type;
    bool zoom, move;
    float x, y, height;
};

/// @brief Structure of objects that stored as the array in the Level class
struct JsonPObj
{
    /// @brief What type is this object? Can be "box", "circle" and etc.
    std::string type;
};

struct JsonPObjPlatform : JsonPObj
{
    float x1, y1, x2, y2;
    uint8_t r, g, b;
};
struct JsonPObjBox : JsonPObj
{
    std::string texture;
    float x, y, w, h, angle, vel_x, vel_y;
};
struct JsonPObjCircle : JsonPObj
{
    float x, y, radius, vel_x, vel_y;
    uint8_t r, g, b;
    uint8_t r_angle, g_angle, b_angle;
};

struct JsonCycle
{
    int delay;
    std::vector<JsonPObj*>* objects = new std::vector<JsonPObj*>();
};

/// @brief Class that represents Sand-Box2D level loaded from JSON. 
/// Its fields should be handled by some level loader.
/// For further info refer to the JSON level file documentation:
/// https://github.com/Hammerill/Sand-Box2D-levels/
class Level
{
private:
    float getRandomFloat(float min, float max);

    /// @brief Load number (parse it), further info here:
    /// https://github.com/Hammerill/Sand-Box2D-levels/blob/main/docs/README-level-file.md#numbers-input
    /// @param input JSON value to be parsed. It can be number itself or string random ("100-200")
    /// @return NULL if something went wrong, otherwise number represented as float. 
    float LoadNumber(Json::Value input);

    /// @brief Array of loaded cycles from JSON represented as structs.
    /// Should be accessed through getter (see getCycles()).
    std::vector<JsonCycle*>* cycles = new std::vector<JsonCycle*>();

    /// @brief Parse PObject and add it to array.
    /// @param jsonObj PObj from JSON to be proceeded.
    /// @param arrayPObjs array to which add parsed PObj.
    void ProceedPObj(Json::Value jsonObj, std::vector<JsonPObj*>* arrayPObjs);
public:
    ~Level();

    /// @brief Represents "metadata" sector from the file.
    /// Returned as Json::Value because it can contain custom data.
    Json::Value metadata;

    /// @brief Represents "options" sector from the file.
    JsonOptions options;

    /// @brief Represents "camera" sector from the file.
    JsonCamera camera;

    /// @brief Array of loaded objects from JSON represented as structs.
    std::vector<JsonPObj*>* objects = new std::vector<JsonPObj*>();

    /// @brief Get array of loaded cycles from JSON represented as structs.
    /// Should be accessed through getter because it should return random values every iteration.
    std::vector<JsonCycle*>* getCycles();

    /// @brief Load JSON file and represent it as an array of structs (see Level::objects).
    /// @param base where is directory of all the levels stored on the platform are located?
    /// @param filepath where is the level file (*.json) located relatively to the "base"?
    /// Will load this file.
    /// @return exception if something went wrong, nullptr if everything's ok.
    std::exception* LoadFile(std::string base, std::string filepath);
};
