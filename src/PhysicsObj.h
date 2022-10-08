#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <box2d/box2d.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

class PhysicsObj
{
protected:
    const float RAD2DEG = 180 / M_PI;

    SDL_Texture* texture;

    b2World* world;

    b2Body* body;
    b2BodyDef bodyDef;
    b2Vec2 vel;
    b2FixtureDef fixtureDef;

public:
    ~PhysicsObj();

    void SetSprite(SDL_Renderer* renderer, const char* path_to_texture);
    b2Body* getBody();

    virtual void Reset() = 0;
    virtual void Render(SDL_Renderer* renderer, float x_offset, float y_offset, float zoom) = 0;
};
