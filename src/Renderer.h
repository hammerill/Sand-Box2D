#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "FontManager.h"

/// @brief Window modes for game (fullscreen or no and etc.).
enum WindowMode
{
    WINDOWED = 0,           // Game will run in the window. When resizing, place window at the center.
    FULLSCREEN_SIMPLE,      // Game will run in simple fullscreen mode.
    FULLSCREEN_REAL         // Game will run in real fullscreen mode (it can change video mode at running machine).
};

/// @brief Everything you need to describe window.
struct WindowParams
{
    /// @brief Defines game window mode (see enum WindowMode).
    WindowMode mode = FULLSCREEN_SIMPLE;

    /// @brief Desired size of the window.
    /// If you're going to fullscreen and leave 0, it considered as size of the screen.
    int width = 0, height = 0;

    /// If go fullscreen, how much we should scale game window?
    ///
    /// If 1 - don't scale (pixel to pixel).
    /// If 2 - each 2x2 pixels sector on the screen will represent 1 pixel from the game.
    int scale = 1;
};

/// @brief Class that manages window and its renderer.
class Renderer
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int window_width, window_height;

    WindowMode window_mode = FULLSCREEN_SIMPLE;

    const char* path_to_icon;

    Font* font = new Font();

public:
    ~Renderer();

    /// @brief Init video.
    /// @param params declare settings of your window here (see struct WindowParams).
    /// You can change them later using ChangeRes().
    /// @param path_to_font path to the font (*.ttf) file which will be always rendered.
    /// Leave nullptr if you don't want to render any text.
    /// @param path_to_icon path to the icon that will be set to program instance.
    /// Leave nullptr if you're setting icon in CMake.
    void InitVideo(WindowParams params, const char* path_to_font = nullptr, const char* path_to_icon = nullptr);

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();

    /// @brief Change game resolution and go/exit fullscreen if need.
    /// @param params declare settings of your window here (see struct WindowParams).
    void ChangeRes(WindowParams params);

    /// @brief Render text. Will use internal Font object to do that
    /// (pass this function into it).
    /// @param renderer the rendering context.
    /// @param text text to be rendered.
    /// @param x X position of the beginning of the text.
    /// @param y Y position of the beginning of the text.
    /// @param scale How much scale font? Also multiplied by const FontWidth.
    /// @param r Color.
    /// @param g Color.
    /// @param b Color.
    void RenderText(SDL_Renderer* renderer, const char* text, int x = 0, int y = 0, float scale = 1, Uint8 r = 0xFF, Uint8 g = 0xFF, Uint8 b = 0xFF);

    /// @brief Get pointer to Font object which is used to render text by default.
    Font* GetFont();

    WindowParams GetWindowParams();
};
