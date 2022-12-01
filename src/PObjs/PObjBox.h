#pragma once

#include "BasePObj.h"

struct BoxDesc
{
    float x = 0, y = 0;
    float w = 1, h = 1;
    float angle = 0;
    float vel_x = 0, vel_y = 0;
};

/// Child class of the BasePObj which represents either box or 
/// rectangle in the WorldManager environment.
class PObjBox: public BasePObj
{
private:
    /// @brief What values you should declare for create Box?
    /// Used only while creating Box.
    BoxDesc boxDesc;

    /// @brief Shape of the box entity.
    b2PolygonShape boxShape;

public:
    PObjBox(BoxDesc boxDesc);
    ~PObjBox();

    /// @brief Set some parameter of this Box. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted. Can be any type (int, std::string, etc...)
    template<typename T>
    void SetParam(std::string name, T value);

    /// @brief Get some parameter of this Box. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return value of any type. Call like this - Box.GetParam<float>("x");
    template<typename T>
    T GetParam(std::string name);

    /// Register this box in the world and set its texture. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where box should be registered.
    /// @param renderer the rendering context.
    void Register(b2World* world, SDL_Renderer* renderer);

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
