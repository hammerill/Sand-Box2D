#include <iomanip>
#include <box2d/box2d.h>
#include <iostream>
#include <cmath>
#include <tuple>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Controls.h"
#include "BoxEntity.h"

#ifdef Linux
const bool isLinux = true;
#else
const bool isLinux = false;
#endif

class WorldEntity
{
private:

public:
    b2World* world;
    WorldEntity()
    {
        this->world = new b2World(b2Vec2(0.0f, 9.81f));
    }
    ~WorldEntity()
    {

    }
};


//LEGACY TO REMOVE LATER
const int MET2PIX = 80; // 640 / 80 = 8

const int WIDTH = 960;
const int HEIGHT = 540;

const int SCALED_WIDTH = WIDTH / MET2PIX; // 4 | 3
const int SCALED_HEIGHT = HEIGHT / MET2PIX;

//1 rad × 180/π = 57,296°
const float RAD2DEG = 180 / M_PI;
/////////////////////

bool isRunning = true;

void Update(BoxEntity* box, b2World* world, unsigned int a = NULL, unsigned int b = NULL)
{
    if (isLinux)
        b = a;

    Ctrl::Check();
    if (Ctrl::getIsExit())
    {
        isRunning = false;
    }                        

    box->Step();
    world->Step(1.0f / 60.0f, 6.0f, 2.0f);
}
void Render(BoxEntity* box, SDL_Renderer *renderer, b2EdgeShape edgeShape)
{
    SDL_SetRenderDrawColor(renderer, 0x32, 0x32, 0x32, 0);
    SDL_RenderClear(renderer);

    if (Ctrl::getIsReset())
    {
        box->Reset();
    }

    box->Render();

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
    SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex1.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex1.y) * MET2PIX, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex2.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex2.y) * MET2PIX);
    
    SDL_RenderPresent(renderer);
}

int main(int argv, char** args)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Box2D", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    b2World* world = new b2World(b2Vec2(0.0f, 9.81f)); // new b2World(b2Vec2(0.0f, 9.81f));
    
    // cartesian origin
    float ground_x = 4.0f;
    float ground_y = 4.0f;
    
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = 1.0f;
    startpoint.y = 2.0;
    
    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3;
    endpoint.y = 2.0;
    
    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y); // set the starting position x and y cartesian
    myGroundDef.angle = 0;
    
    b2Body* groundLineBody = world->CreateBody(&myGroundDef);
    
    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided( startpoint, endpoint ); // length -> coordinate vector from to vector
    
    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    groundLineBody->CreateFixture(&edgeFixtureDef);

    // cartesian origin of _0019_PLATF.png 89 x 22
    float x_plat = -3.6f; // to edge
    float y_plat = -0.14f;// to edge
    
    // size of the platform
    float w_plat = 89.0f / MET2PIX;
    float h_plat = 22.0f / MET2PIX;
    
    // define a Rect for this platform and its body def
    SDL_Rect platform;
    b2Body* Body_platform;
    
    b2BodyDef platformBodyDef;
    platformBodyDef.type = b2_staticBody;
    platformBodyDef.position.Set(x_plat, y_plat);
    
    Body_platform = world->CreateBody(&platformBodyDef);
    
    b2PolygonShape platformTile;    // subtracting radius fixes the incorrect little gap that can appear when working with really small resolutions
    platformTile.SetAsBox((w_plat / 2.0f) - platformTile.m_radius, (h_plat / 2.0f) - platformTile.m_radius); // subtracting the radius kills the gap issue:
    b2FixtureDef fixturePlat;
    fixturePlat.shape = &platformTile;
    fixturePlat.density = 1.0f;
    fixturePlat.friction = 0.3f;
    fixturePlat.restitution = 0.5f;
    Body_platform->CreateFixture(&fixturePlat);
    
    // set the SDL_RECT rendering values
    platform.w = w_plat * MET2PIX;
    platform.h = h_plat * MET2PIX;
    platform.x = ((SCALED_WIDTH / 2.0f) + x_plat) * MET2PIX - platform.w / 2;
    platform.y = ((SCALED_HEIGHT / 2.0f) + y_plat) * MET2PIX - platform.h / 2;

    BoxEntity* box = new BoxEntity(world, renderer, IMG_Load("assets/box.png"), 4.0, 0, 3, 3, 45.0f);

    unsigned int a, b = 0;
    double delta = 0;

    while (isRunning)
    {
        if (isLinux)
        {
            a = SDL_GetTicks();
            delta = a - b;

            if (delta > 1000/60.0)
            {
                Update(box, world, a, b);
                Render(box, renderer, edgeShape);
            }
        }
        else
        {
            Update(box, world);
            Render(box, renderer, edgeShape);
        }
    }
    return 0;
}
