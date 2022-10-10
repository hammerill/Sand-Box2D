#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <box2d/box2d.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

/// @brief Base class for physics objects in the WorldManager environment.
class PhysicsObj
{
protected:
    /// @brief By multiplying this with value in radians
    /// you will get a value in degrees.
    /// @example RAD * RAD2DEG == DEG
    /// @example DEG / RAD2DEG == RAD
    const float RAD2DEG = 180 / M_PI;

    /// @brief Texture of the physics object.
    SDL_Texture* texture;

    /// @brief The world where physics object exists.
    b2World* world;

    /// @brief Main body of the physics object.
    b2Body* body;
    /// @brief Body definition of the physics object.
    b2BodyDef bodyDef;
    /// @brief Starting velocity of the physics object.
    b2Vec2 vel;
    /// @brief Fixture definition of the physics object.
    b2FixtureDef fixtureDef;

    /// @brief Path to the image file of texture.
    const char* pathToTexture;

public:
    ~PhysicsObj();
    
    /// Register this ph. obj. in the world and set its texture. Should be 
    /// called only when no world calculations are performing.
    /// @param world link to the world where ph. obj. should be registered.
    /// @param renderer the rendering context.
    virtual void Register(b2World* world, SDL_Renderer* renderer) = 0;

    /// @brief Load a texture for physics object.
    /// @param renderer the rendering context.
    void LoadTexture(SDL_Renderer* renderer);

    b2Body* getBody();

    /// @brief Set default position and angle of physics object.
    virtual void Reset() = 0;
    
    /// @brief Render this physics object.
    /// @param renderer the rendering context.
    /// @param x_offset camera X offset in pixels.
    /// @param y_offset camera Y offset in pixels.
    /// @param zoom camera zoom coefficient.
    virtual void Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom) = 0;
};
