#include "FontManager.h"

Font::Font()
{
    Font::loaded = false;
}

Font::~Font()
{
    if (Font::loaded)
        TTF_CloseFont(Font::font);
}

void Font::LoadFont(const char* path_to_font)
{
    if (Font::loaded)
        TTF_CloseFont(Font::font);
    
    try
    {
        TTF_Init();

        Font::font = TTF_OpenFont(path_to_font, Font::FontWidth);
        Font::loaded = true;
    }
    catch (const std::exception& e)
    {
        TTF_CloseFont(Font::font);
        Font::loaded = false;
    }
}

void Font::Render(SDL_Renderer* renderer, const char* text, int x, int y, float scale, Uint8 r, Uint8 g, Uint8 b)
{
    if (Font::loaded)
    {
        SDL_Rect textRect;

        TTF_SizeText(Font::font, text, &(textRect.w), &(textRect.h));
        textRect.x = x;
        textRect.y = y;

        textRect.w *= scale;
        textRect.h *= scale;

        SDL_Surface* textSurface = TTF_RenderText_Solid(Font::font, text, {r, g, b});
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

bool Font::GetLoaded()  { return Font::loaded; }
