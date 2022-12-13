#pragma once

#include "BasePObj.h"

struct PlatformDesc
{
    float x1 = 0, y1 = 0;
    float x2 = 0, y2 = 0;
};

/// Child class of the BasePObj which represents static yellow stick. 
/// Or main platform.
class PObjPlatform: public BasePObj
{
private:
    /// @brief What values you should declare for create Platform?
    /// Used only while creating it.
    PlatformDesc platformDesc;

    /// @brief Color.
    uint8_t r, g, b;

    /// @brief Shape of the Platform.
    b2EdgeShape platformShape;

public:
    PObjPlatform(PlatformDesc platformDesc);
    ~PObjPlatform();

    /// @brief Set some parameter of this Platform. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted.
    void SetParam(std::string name, Json::Value value);

    /// @brief Get some parameter of this Platform. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return Json::Value. Call its functions like AsFloat(), etc.
    Json::Value GetParam(std::string name);
    
    /// Register this Platform in the world. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where Platform should be registered.
    /// @param renderer the rendering context.
    /// @param textures textures vector to work with.
    void Register(b2World* world, SDL_Renderer* renderer, std::map<std::string, SDL_Texture*>& textures);

    /// @brief Render this Platform.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if Platform rendered. False if didn't render because it's out of screen bounds. 
    bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width = 0, int height = 0);
};
