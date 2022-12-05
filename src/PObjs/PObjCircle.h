#pragma once

#include "BasePObj.h"

#include <SDL2/SDL2_gfxPrimitives.h>

struct CircleDesc
{
    float x = 0, y = 0;
    float radius = 0;
    float angle = 0;
    float vel_x = 0, vel_y = 0;
    float vel_ang = 0;
};

/// Child class of the BasePObj which represents Circle 
/// in the WorldManager environment.
class PObjCircle: public BasePObj
{
private:
    /// @brief What values you should declare for create Circle?
    /// Used only while creating Circle.
    CircleDesc circleDesc;

    /// @brief Color of the Circle.
    uint8_t r, g, b;

    /// @brief Color of the Circle angle renderer.
    uint8_t r_angle, g_angle, b_angle;

    /// @brief Shape of the Circle.
    b2CircleShape circleShape;

    /// @brief Starting velocity of the Circle.
    b2Vec2 vel;
    /// @brief Starting angular velocity of the Circle.
    float vel_ang;

public:
    PObjCircle(CircleDesc circleDesc);
    ~PObjCircle();

    /// @brief Set some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted.
    void SetParam(std::string name, Json::Value value);

    /// @brief Get some parameter of this Circle. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return Json::Value. Call its functions like AsFloat(), etc.
    Json::Value GetParam(std::string name);
    
    /// Register this Circle in the world. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where Circle should be registered.
    /// @param renderer the rendering context.
    void Register(b2World* world, SDL_Renderer* renderer = nullptr);

    /// @brief Render this Circle.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if Circle rendered. False if didn't render because it's out of screen bounds. 
    bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width = 0, int height = 0);
};
