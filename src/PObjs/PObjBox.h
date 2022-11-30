#pragma once

#include "BasePObj.h"

/// Child class of the BasePObj which represents either box or 
/// rectangle in the WorldManager environment.
class PObjBox: public BasePObj
{
private:
    /// Default values of position and angle of box entity.
    /// Used while creating a box or resetting it to default.
    float x, y, w, h, angle, vel_x, vel_y;

    /// @brief Shape of the box entity.
    b2PolygonShape boxShape;

public:
    /// @brief Create a new box entity.
    /// @param path_to_texture path to the image file of texture.
    /// @param x_box position X of the center of the box in Box2D meters.
    /// @param y_box position Y of the center of the box in Box2D meters.
    /// @param w_box width of the box in Box2D meters.
    /// @param h_box height of the box in Box2D meters.
    /// @param angle_box angle of the box in radians.
    /// @param vel_x velocity on X of the box.
    /// @param vel_y velocity on Y of the box.
    PObjBox(const char* path_to_texture, float x_box, float y_box, float w_box, float h_box, float angle_box, float vel_x = 0, float vel_y = 0);
    
    /// @brief Create a new box entity.
    /// @param texture link to the texture. Can be used in order to improve perfomance with many boxes with same texture.
    /// @param x_box position X of the center of the box in Box2D meters.
    /// @param y_box position Y of the center of the box in Box2D meters.
    /// @param w_box width of the box in Box2D meters.
    /// @param h_box height of the box in Box2D meters.
    /// @param angle_box angle of the box in radians.
    /// @param vel_x velocity on X of the box.
    /// @param vel_y velocity on Y of the box.
    PObjBox(SDL_Texture* texture, float x_box, float y_box, float w_box, float h_box, float angle_box, float vel_x = 0, float vel_y = 0);
    
    ~PObjBox();

    /// @brief Set some parameter of this Box. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted. Can be any type (int, const char*, etc...)
    template<typename T>
    void SetParam(const char* name, T value);

    /// @brief Get some parameter of this Box. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return value of any type. Call like this - Box.GetParam<float>("x");
    template<typename T>
    T GetParam(const char* name);

    /// Register this box in the world and set its texture. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where box should be registered.
    /// @param renderer the rendering context.
    void Register(b2World* world, SDL_Renderer* renderer);

    /// @brief Set default position and angle of the box.
    void Reset();

    /// @brief Render this box.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if box rendered. False if didn't render because it's out of screen bounds. 
    bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width = 0, int height = 0);
};
