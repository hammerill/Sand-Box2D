#pragma once

#include "BasePObj.h"

#include <SDL2/SDL2_gfxPrimitives.h>

/// Child class of the BasePObj which represents circle 
/// in the WorldManager environment.
class PObjCircle: public BasePObj
{
private:
    /// Default values of position and radius of circle entity.
    /// Used while creating a circle or resetting it to default.
    float x, y, radius, vel_x, vel_y;

    /// @brief Color of the circle entity.
    uint8_t r, g, b;

    /// @brief Color of the circle entity angle renderer.
    uint8_t r_angle, g_angle, b_angle;

    /// @brief Shape of the circle entity.
    b2CircleShape circleShape;

public:
    /// @brief Create a new circle entity.
    /// @param x position X of the circle center in Box2D meters.
    /// @param y position Y of the circle center in Box2D meters.
    /// @param radius radius of the circle in Box2D meters.
    /// @param vel_x velocity on X of the circle.
    /// @param vel_y velocity on Y of the circle.
    /// @param r color RED.
    /// @param g color GREEN.
    /// @param b color BLUE.
    /// @param r_angle color RED of angle render.
    /// @param g_angle color GREEN of angle render.
    /// @param b_angle color BLUE of angle render.
    PObjCircle( float x, float y, float radius, float vel_x = 0, float vel_y = 0,
                uint8_t r = 0xFF,
                uint8_t g = 0x80,
                uint8_t b = 0xFF,
                uint8_t r_angle = 0,
                uint8_t g_angle = 0,
                uint8_t b_angle = 0);
                
    ~PObjCircle();

    /// @brief Set some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted. Can be any type (int, const char*, etc...)
    template<typename T>
    void SetParam(const char* name, T value);

    /// @brief Get some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return value of any type. Call like this - Circle.GetParam<float>("x");
    template<typename T>
    T GetParam(const char* name);
    
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
