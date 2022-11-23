#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

    /// @brief if go fullscreen, how much we should scale game to screen?
    /// If 1 - don't scale (pixel to pixel). If 2 - each 2x2 pixels sector in the screen
    /// will represent 1 pixel from the game.
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

public:
    ~Renderer();

    /// @brief Init video.
    /// @param params declare settings of your window here (see struct WindowParams).
    /// You can change them later using ChangeRes().
    /// @param path_to_icon path to the icon that will be set to program instance.
    /// Leave nullptr if you're setting icon in CMake.
    void InitVideo(WindowParams params, const char* path_to_icon = nullptr);

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();

    /// @brief Change game resolution and go/exit fullscreen if need.
    /// @param params declare settings of your window here (see struct WindowParams).
    void ChangeRes(WindowParams params);

    WindowMode GetWindowMode();

    int GetWidth();
    int GetHeight();
};
