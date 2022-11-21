#pragma once

#include <SDL2/SDL_ttf.h>

/// @brief Static class for rendering text. Intended for one single font.
class Font
{
private:
    static TTF_Font* font;

    static bool loaded;

public:
    Font();
    ~Font();

    /// @brief Load font to private field of this class, then you can render it later with Render().
    /// @param path_to_font path to the font (*.ttf) file.
    static void LoadFont(const char* path_to_font);

    /// @brief Render text.
    /// @param renderer the rendering context.
    /// @param text text to be rendered.
    /// @param x X position of the beginning of the text.
    /// @param y Y position of the beginning of the text.
    /// @param scale How much scale font? Also multiplied by const FontWidth.
    /// @param r Color.
    /// @param g Color.
    /// @param b Color.
    static void Render(SDL_Renderer* renderer, const char* text, int x = 0, int y = 0, float scale = 1, Uint8 r = 0xFF, Uint8 g = 0xFF, Uint8 b = 0xFF);

    /// @brief Did FontManager load font and is it able to render text?
    static bool getLoaded();

    /// @brief Constant scale of font width. If you use scale 1 at Render(), it will use scale 1 * FontWidth (8)
    /// If 2, 2 * FontWidth (16) and etc.
    static const int FontWidth = 8;
};
