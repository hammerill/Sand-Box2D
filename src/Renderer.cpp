#include "Renderer.h"

Renderer::Renderer(const char* path_to_icon)
{
    Renderer::InitVideo(path_to_icon);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(Renderer::renderer);
    SDL_DestroyWindow(Renderer::window);
}

void Renderer::InitVideo(const char* path_to_icon)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    
    Renderer::window = SDL_CreateWindow("Sand-Box2D", 0, 0, 0, 0, SDL_WINDOW_SHOWN);

    if (path_to_icon != nullptr)
    {
        SDL_Surface* icon = IMG_Load(path_to_icon);
        SDL_SetWindowIcon(Renderer::window, icon);
    }

    Renderer::renderer = SDL_CreateRenderer(Renderer::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Renderer::ChangeRes();
}

SDL_Renderer* Renderer::GetRenderer()   { return renderer; }
SDL_Window* Renderer::GetWindow()       { return window; }

void Renderer::ChangeRes(WindowMode mode, int width, int height, int fullscreen_scale)
{
    switch (mode)
    {
    case WINDOWED:
        Renderer::window_width = width;
        Renderer::window_height = height;

        SDL_SetWindowSize(Renderer::window, Renderer::window_width, Renderer::window_height);
        SDL_RenderSetLogicalSize(Renderer::renderer, Renderer::window_width, Renderer::window_height);

        SDL_SetWindowFullscreen(Renderer::window, 0);

        SDL_SetWindowPosition(Renderer::window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        break;
    case FULLSCREEN_SIMPLE:
    case FULLSCREEN_REAL:
        if (width == 0 && height == 0)
        {
            SDL_DisplayMode dm;
            SDL_GetCurrentDisplayMode(0, &dm);
            Renderer::window_width = dm.w;
            Renderer::window_height = dm.h;
        }
        else
        {
            Renderer::window_width = width;
            Renderer::window_height = height;
        }

        SDL_SetWindowSize(Renderer::window, Renderer::window_width * fullscreen_scale, Renderer::window_height * fullscreen_scale);

        SDL_RenderSetLogicalSize(Renderer::renderer, Renderer::window_width, Renderer::window_height);
        
        if (mode == FULLSCREEN_SIMPLE)
            SDL_SetWindowFullscreen(Renderer::window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        else
            SDL_SetWindowFullscreen(Renderer::window, SDL_WINDOW_FULLSCREEN);
        
        break;
    default:
        break;
    }
}

WindowMode Renderer::GetWindowMode()    { return window_mode; }

int Renderer::GetWidth()    { return window_width; }
int Renderer::GetHeight()   { return window_height; }
