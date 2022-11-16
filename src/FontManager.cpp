#include "FontManager.h"

TTF_Font* Font::font;

bool Font::loaded = false;

void Font::LoadFont(const char* path_to_font)
{
    TTF_Init();

    Font::font = TTF_OpenFont(path_to_font, Font::FontWidth);
    Font::loaded = true;
}

void Font::Render(SDL_Renderer* renderer, const char* text, int x, int y, int scale, Uint8 r, Uint8 g, Uint8 b)
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

bool Font::getLoaded()  { return Font::loaded; }
