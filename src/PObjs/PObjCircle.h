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
    Uint32 color;

    /// @brief Shape of the circle entity.
    b2CircleShape circleShape;

public:
    /// @brief Create a new circle entity.
    /// @param x position X of the circle center in Box2D meters.
    /// @param y position Y of the circle center in Box2D meters.
    /// @param radius radius of the circle in Box2D meters.
    /// @param vel_x velocity on X of the circle.
    /// @param vel_y velocity on Y of the circle.
    /// @param color color of the circle. Format - 0xRRGGBBAA.
    PObjCircle(float x, float y, float radius, float vel_x = 0, float vel_y = 0, Uint32 color = 0xFFFF80FF);

    ~PObjCircle();
    
    /// Register this circle in the world. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where circle should be registered.
    /// @param renderer the rendering context.
    void Register(b2World* world, SDL_Renderer* renderer = nullptr);

    /// @brief Set default position of the circle.
    void Reset();

    /// @brief Render this circle.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    void Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom);
};
