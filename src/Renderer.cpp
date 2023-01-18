#include "Renderer.h"

Renderer::~Renderer()
{
    SDL_DestroyRenderer(Renderer::renderer);
    SDL_DestroyWindow(Renderer::window);
}

void Renderer::InitVideo(WindowParams params, const char* path_to_sfx_base, const char* path_to_font, const char* path_to_icon)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    
    Renderer::window = SDL_CreateWindow("Sand-Box2D", 0, 0, 0, 0, SDL_WINDOW_SHOWN);

    if (path_to_sfx_base != nullptr)
        Renderer::sounds->LoadSfx(path_to_sfx_base);

    if (path_to_font != nullptr)
        Renderer::font->LoadFont(path_to_font);

    if (path_to_icon != nullptr)
    {
        SDL_Surface* icon = IMG_Load(path_to_icon);
        SDL_SetWindowIcon(Renderer::window, icon);
    }

    Renderer::renderer = SDL_CreateRenderer(Renderer::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Renderer::ChangeRes(params);
}

SDL_Renderer* Renderer::GetRenderer()   { return renderer; }
SDL_Window* Renderer::GetWindow()       { return window; }
SoundManager* Renderer::GetSounds()     { return sounds; }

void Renderer::ChangeRes(WindowParams params)
{
    Renderer::window_mode = params.mode;
    switch (Renderer::window_mode)
    {
    case WINDOWED:
        Renderer::window_width = params.width;
        Renderer::window_height = params.height;

        SDL_SetWindowSize(Renderer::window, Renderer::window_width, Renderer::window_height);
        SDL_RenderSetLogicalSize(Renderer::renderer, Renderer::window_width, Renderer::window_height);

        SDL_SetWindowFullscreen(Renderer::window, 0);

        SDL_SetWindowPosition(Renderer::window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        break;
    case FULLSCREEN_SIMPLE:
    case FULLSCREEN_REAL:
        if (params.width == 0 || params.height == 0)
        {
            SDL_DisplayMode dm;
            SDL_GetCurrentDisplayMode(0, &dm);
            Renderer::window_width = dm.w;
            Renderer::window_height = dm.h;
        }
        else
        {
            Renderer::window_width = params.width;
            Renderer::window_height = params.height;
        }

        SDL_SetWindowSize(Renderer::window, Renderer::window_width * params.scale, Renderer::window_height * params.scale);

        SDL_RenderSetLogicalSize(Renderer::renderer, Renderer::window_width, Renderer::window_height);
        
        SDL_SetWindowFullscreen(Renderer::window,
                                params.mode == FULLSCREEN_SIMPLE ?
                                SDL_WINDOW_FULLSCREEN_DESKTOP :
                                SDL_WINDOW_FULLSCREEN);
        
        break;
    default:
        break;
    }
}

void Renderer::RenderText(const char* text, int x, int y, float scale, bool center, Uint8 r, Uint8 g, Uint8 b)
{
    Renderer::font->Render(Renderer::renderer, text, x, y, scale, center, r, g, b);
}
Font* Renderer::GetFont()
{
    return Renderer::font;
}

WindowParams Renderer::GetWindowParams()    { return {window_mode, window_width, window_height}; }
