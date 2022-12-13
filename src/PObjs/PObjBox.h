#pragma once

#include "BasePObj.h"

struct BoxDesc
{
    float x = 0, y = 0;
    float w = 1, h = 1;
    float angle = 0;
    float vel_x = 0, vel_y = 0;
    float vel_ang = 0;
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

    /// @brief Starting velocity of the Box.
    b2Vec2 vel;
    /// @brief Starting angular velocity of the Box.
    float vel_ang;

public:
    PObjBox(BoxDesc boxDesc);
    ~PObjBox();

    /// @brief Set some parameter of this Box. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted.
    void SetParam(std::string name, Json::Value value);

    /// @brief Get some parameter of this Box. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return Json::Value. Call its functions like AsFloat(), etc.
    Json::Value GetParam(std::string name);

    /// Register this box in the world and set its texture. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where box should be registered.
    /// @param renderer the rendering context.
    /// @param textures textures vector to work with.
    void Register(b2World* world, SDL_Renderer* renderer, std::map<std::string, SDL_Texture*> textures);

    /// @brief Render this box.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if box rendered. False if didn't render because it's out of screen bounds. 
    bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width, int height);
};
