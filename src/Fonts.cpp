#include "Fonts.h"

Font::Font()
{
    Font::loaded = false;
}

Font::~Font()
{
    if (Font::loaded)
        TTF_CloseFont(Font::font);
}

Font::Font(const char* path_to_font)
{
    Font::loaded = false;
    Font::LoadFont(path_to_font);
}

void Font::LoadFont(const char* path_to_font, int font_width)
{
    if (Font::loaded)
        TTF_CloseFont(Font::font);
    
    try
    {
        TTF_Init();

        Font::font = TTF_OpenFont(path_to_font, font_width);
        Font::loaded = true;
    }
    catch (const std::exception& e)
    {
        TTF_CloseFont(Font::font);
        Font::loaded = false;
    }
}

void Font::Render(SDL_Renderer* renderer, const char* text, int x, int y, float scale, bool center, Uint8 r, Uint8 g, Uint8 b)
{
    if (Font::loaded)
    {
        SDL_Rect textRect = Font::GetTextDimensions(text, scale);

        textRect.x = x;
        textRect.y = y;

        if (center)
        {
            textRect.x -= textRect.w / 2;
            textRect.y -= textRect.h / 2;
        }

        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(Font::font, text, {r, g, b});
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

bool Font::GetLoaded()      { return Font::loaded; }
TTF_Font* Font::GetFont()   { return Font::font; }

SDL_Rect Font::GetTextDimensions(const char* text, float scale)
{
    SDL_Rect textRect {0, 0};
    TTF_SizeUTF8(Font::font, text, &(textRect.w), &(textRect.h));

    textRect.w *= scale;
    textRect.h *= scale;

    return textRect;
}
