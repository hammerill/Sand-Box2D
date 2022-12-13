#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <box2d/box2d.h>
#include <jsoncpp/json/value.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

/// @brief Base class for physics objects in the WorldManager environment.
class BasePObj
{
protected:
    /// By multiplying this with value in radians you will get value in degrees.
    ///
    /// RAD * RAD2DEG == DEG
    /// DEG / RAD2DEG == RAD
    const float RAD2DEG = 180 / M_PI;

    /// @brief ID of this PObj.
    int id = -1;

    /// @brief Texture of the PObj.
    SDL_Texture* texture = nullptr;

    /// @brief Path to texture of the PObj. Will be loaded while calling Register().
    /// Can be accessed through SetParam() and GetParam() as "texture_path".
    std::string texture_path;

    /// @brief Link to the main body of the PObj.
    b2Body* body;
    /// @brief Body definition of the PObj.
    b2BodyDef bodyDef;
    /// @brief Fixture definition of the PObj.
    b2FixtureDef fixtureDef;

    /// @brief Load texture. If it was loaded before, will return pointer to already loaded texture.
    /// @param textures textures vector to work with.
    /// @param path path to the texture to be loaded.
    /// @param renderer the rendering context.
    /// @return pointer to the SDL texture. nullptr if can't load.
    SDL_Texture* LoadTexture(std::map<std::string, SDL_Texture*>& textures, std::string path, SDL_Renderer* renderer);

public:
    virtual ~BasePObj() {};

    /// @brief Set some parameter of this PObj. For example, X or Y pos.
    /// @param name name of the parameter to be setted. For example, "x" or "y".
    /// @param value value of the parameter to be setted.
    virtual void SetParam(std::string name, Json::Value value) = 0;

    /// @brief Get some parameter of this PObj. For example, X or Y pos.
    /// @param name name of the parameter to be getted. For example, "x" or "y".
    /// @return Json::Value. Call its functions like AsFloat(), etc.
    virtual Json::Value GetParam(std::string name) = 0;
    
    /// Register this PObj in the world and set its parameters. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where PObj should be registered.
    /// @param renderer the rendering context.
    /// @param textures textures vector to work with.
    virtual void Register(b2World* world, SDL_Renderer* renderer, std::map<std::string, SDL_Texture*>& textures) = 0;

    b2Body* GetBody();
    int GetId();
    
    /// @brief Render this PObj.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    /// @param width screen width in pixels.
    /// @param height screen height in pixels.
    /// @return true if object rendered. False if didn't render because it's out of screen bounds. 
    virtual bool Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom, int width = 0, int height = 0) = 0;
};
