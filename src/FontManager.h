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

    /// @brief Loads font to private field of this class, then you can render it later with Render().
    /// @param path_to_font filepath to the font (*.ttf) file.
    /// @param fontSize size of the text.
    static void LoadFont(const char* path_to_font, int fontSize = 8);

    /// @brief Renders text.
    /// @param renderer the rendering context.
    /// @param text text to be rendered.
    /// @param x X position of the beginning of the text.
    /// @param y Y position of the beginning of the text.
    /// @param r Color.
    /// @param g Color.
    /// @param b Color.
    static void Render(SDL_Renderer* renderer, const char* text, int x = 0, int y = 0, Uint8 r = 0xFF, Uint8 g = 0xFF, Uint8 b = 0xFF);

    /// @brief Did FontManager load font and is it able to render text?
    static bool getLoaded();
};
