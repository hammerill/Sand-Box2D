#pragma once

#include <SDL2/SDL_ttf.h>
#include <exception>

/// @brief Class for rendering text. Intended for one single font.
class Font
{
private:
    TTF_Font* font;

    bool loaded;

public:
    Font();
    ~Font();

    /// @brief Load font to private field of this class, then you can render it later with Render().
    /// If can't load font it will just leave "loaded" field "false", so it just won't render text.
    /// @param path_to_font path to the font (*.ttf) file.
    void LoadFont(const char* path_to_font);

    /// @brief Render text.
    /// @param renderer the rendering context.
    /// @param text text to be rendered.
    /// @param x X position of the beginning of the text.
    /// @param y Y position of the beginning of the text.
    /// @param scale How much scale font? Also multiplied by const FontWidth.
    /// @param r Color.
    /// @param g Color.
    /// @param b Color.
    void Render(SDL_Renderer* renderer, const char* text, int x = 0, int y = 0, float scale = 1, Uint8 r = 0xFF, Uint8 g = 0xFF, Uint8 b = 0xFF);

    /// @brief Did FontManager load font and is it able to render text?
    bool GetLoaded();

    /// @brief Constant scale of font width. If you use scale 1 at Render(), it will use scale 1 * FontWidth (8)
    /// If 2, 2 * FontWidth (16) and etc.
    const int FontWidth = 8;
};
