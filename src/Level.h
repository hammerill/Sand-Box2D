#pragma once

#include <json/json.h>

#include <iostream>
#include <fstream>
#include <vector>

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

/// @brief Class that represents Sand-Box2D level loaded from JSON. 
/// Its fields should be handled by some level loader.
class Level
{
private:

public:
    /// @brief Create a new Level instance.
    /// @param base where is directory of all the levels stored on the platform are located?
    /// @param filepath where is the level file (*.json) located relatively to the "base"?
    /// Will load this file.
    Level(std::string base, std::string filepath);

    /// @brief Array of loaded objects from JSON represented as structs.
    std::vector<JsonPObj>* pObjects = new std::vector<JsonPObj>();

    /// @brief Load JSON file and represent it as an array of structs (see pObjects).
    /// @param base where is directory of all the levels stored on the platform are located?
    /// @param filepath where is the level file (*.json) located relatively to the "base"?
    /// Will load this file.
    void LoadFile(std::string base, std::string filepath);
};
