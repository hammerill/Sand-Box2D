#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846264338327950288  /* pi */
#endif

class BoxEntity
{
private:
    const int MET2PIX = 80;
    const float RAD2DEG = 180 / M_PI;

    SDL_Texture* texture_box;
    SDL_Renderer* renderer;

    float x_box, y_box, w_box, h_box, angle_box;

    SDL_Rect box;

    b2Body* Body;
    b2BodyDef boxBodyDef;
    b2Vec2 vel;

    b2World* world;

    b2FixtureDef fixtureDef;
    b2PolygonShape dynamicBox;

public:
    BoxEntity(b2World* world, SDL_Renderer* renderer, SDL_Surface* tmp_sprites, float x_box, float y_box, float w_box, float h_box, float angle_box);
    ~BoxEntity();

    void SetSprite(SDL_Surface* tmp_sprites);

    void Reset();
    void Step();
    void Render();
};
