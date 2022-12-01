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
    /// Used only while creating Platform.
    PlatformDesc platformDesc;

    /// @brief Color.
    uint8_t r, g, b;

    /// @brief Shape of the platform entity.
    b2EdgeShape platformShape;

public:
    PObjPlatform(PlatformDesc platformDesc);
    ~PObjPlatform();

    /// @brief Set some parameter of this Platform. For example, color.
    /// @param name name of the parameter to be setted. For example, "r", "g" or "b".
    /// @param value value of the parameter to be setted. Can be any type (uint8_t, etc...)
    template<typename T>
    void SetParam(std::string name, T value);

    /// @brief Get some parameter of this Platform. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return value of any type. Call like this - Platform.GetParam<float>("x");
    template<typename T>
    T GetParam(std::string name);

    /// @brief Render this platform.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if platform rendered. False if didn't render because it's out of screen bounds. 
    bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width = 0, int height = 0);
};
