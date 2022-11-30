#pragma once

#include "BasePObj.h"

/// Child class of the BasePObj which represents static yellow stick. 
/// Or main platform.
class PObjPlatform: public BasePObj
{
private:
    /// @brief Start and end positions of platform.
    float x1, y1, x2, y2;

    /// @brief Color.
    uint8_t r, g, b;

    /// @brief Shape of the platform entity.
    b2EdgeShape platformShape;

public:
    /// @brief Create a new platform entity.
    /// @param x1_plat position X of the start point of the platform.
    /// @param y1_plat position Y of the start point of the platform.
    /// @param x2_plat position X of the end point of the platform.
    /// @param y2_plat position Y of the end point of the platform.
    /// @param r color RED.
    /// @param g color GREEN.
    /// @param b color BLUE.
    PObjPlatform(float x1_plat, float y1_plat, float x2_plat, float y2_plat,
                    uint8_t r = 0xFF,
                    uint8_t g = 0xFF,
                    uint8_t b = 0);
    
    ~PObjPlatform();

    /// @brief Set some parameter of this Platform. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted. Can be any type (int, const char*, etc...)
    template<typename T>
    void SetParam(const char* name, T value);

    /// @brief Get some parameter of this Platform. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return value of any type. Call like this - Platform.GetParam<float>("x");
    template<typename T>
    T GetParam(const char* name);

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
