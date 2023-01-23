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
    Font(const char* path_to_font);

    /// @brief Load font to private field of this class, then you can render it later with Render().
    /// If can't load font it will just leave "loaded" field "false", so it just won't render text.
    /// @param path_to_font path to the font (*.ttf) file.
    /// @param font_width open font with which width? Caution, to change font width fully
    /// you have to reload it with this function again. If you just want to scale it use render params.
    void LoadFont(const char* path_to_font, int font_width = 16);

    /// @brief Render text.
    /// @param renderer the rendering context.
    /// @param text text to be rendered.
    /// @param x X position of the beginning of the text.
    /// @param y Y position of the beginning of the text.
    /// @param scale How much scale font? Also multiplied by const FontWidth.
    /// @param center Do we need to center text? If yes, given X and Y would be center of the text.
    /// @param r Color.
    /// @param g Color.
    /// @param b Color.
    void Render(SDL_Renderer* renderer, const char* text, int x = 0, int y = 0, float scale = 1, bool center = false, Uint8 r = 0xFF, Uint8 g = 0xFF, Uint8 b = 0xFF);

    /// @brief Did FontManager load font and is it able to render text?
    bool GetLoaded();

    /// @brief Will return SDL_Rect which will show how much pixels this text takes.
    /// You can leave no parameters to get factual font width and height (of one symbol).
    SDL_Rect GetTextDimensions(const char* text = "-", float scale = 1);
};
