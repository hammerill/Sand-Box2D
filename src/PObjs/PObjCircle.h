#pragma once

#include "BasePObj.h"

#include <SDL2/SDL2_gfxPrimitives.h>

struct CircleDesc
{
    float x = 0, y = 0;
    float radius = 0;
    float angle = 0;
    float vel_x = 0, vel_y = 0;
};

/// Child class of the BasePObj which represents circle 
/// in the WorldManager environment.
class PObjCircle: public BasePObj
{
private:
    /// @brief What values you should declare for create Circle?
    /// Used only while creating Circle.
    CircleDesc circleDesc;

    /// @brief Color of the circle entity.
    uint8_t r, g, b;

    /// @brief Color of the circle entity angle renderer.
    uint8_t r_angle, g_angle, b_angle;

    /// @brief Shape of the circle entity.
    b2CircleShape circleShape;

public:
    PObjCircle(CircleDesc circleDesc);
    ~PObjCircle();

    /// @brief Set some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted. Can be any type (int, std::string, etc...)
    template<typename T>
    void SetParam(std::string name, T value);

    /// @brief Get some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return value of any type. Call like this - Circle.GetParam<float>("x");
    template<typename T>
    T GetParam(std::string name);
    
    /// Register this circle in the world. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where circle should be registered.
    /// @param renderer the rendering context.
    void Register(b2World* world, SDL_Renderer* renderer = nullptr);

    /// @brief Render this circle.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if circle rendered. False if didn't render because it's out of screen bounds. 
    bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width = 0, int height = 0);
};
