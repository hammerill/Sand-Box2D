#pragma once

#include "PhysicsObj.h"

/// Child class of the PhysicsObj which represents static yellow stick. 
/// Or main platform.
class PlatformEntity: public PhysicsObj
{
private:
    /// @brief Start and end positions of platform.
    float x1, y1, x2, y2;

    /// @brief Shape of the platform entity.
    b2EdgeShape platformShape;

public:
    /// @brief Create a new platform entity.
    /// @param x1_plat position X of the start point of the platform.
    /// @param y1_plat position Y of the start point of the platform.
    /// @param x2_plat position X of the end point of the platform.
    /// @param y2_plat position Y of the end point of the platform.
    PlatformEntity(float x1_plat, float y1_plat, float x2_plat, float y2_plat);

    /// Register this platform in the world and set its texture. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where platform should be registered.
    /// @param renderer the rendering context.
    void Register(b2World* world, SDL_Renderer* renderer = nullptr);

    void Reset();

    /// @brief Render this platform.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    void Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom);
};
