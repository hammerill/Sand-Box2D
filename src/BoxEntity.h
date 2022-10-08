#pragma once

#include "PhysicsObj.h"

/// Child class of the PhysicsObj which represents either box or 
/// rectangle in the WorldManager environment.
class BoxEntity: public PhysicsObj
{
private:
    /// Default values of position and angle of box entity.
    /// Used while creating a box or resetting it to default.
    float x, y, w, h, angle;

    /// @brief Shape of the box entity.
    b2PolygonShape boxShape;

public:
    /// @brief Create a new box entity.
    /// @param world the world where box exists.
    /// @param renderer the rendering context.
    /// @param path_to_texture path to the image file of texture.
    /// @param x_box position X of upper left corner of the box in Box2D meters.
    /// @param y_box position Y of upper left corner of the box in Box2D meters.
    /// @param w_box width of the box in Box2D meters.
    /// @param h_box height of the box in Box2D meters.
    /// @param angle_box angle of the box in radians.
    BoxEntity(b2World* world, SDL_Renderer* renderer, const char* path_to_texture, float x_box, float y_box, float w_box, float h_box, float angle_box);
    
    /// @brief Set default position and angle of the box.
    void Reset();

    /// @brief Render this box.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    void Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom);
};
