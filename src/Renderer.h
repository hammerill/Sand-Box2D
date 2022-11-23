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

/// @brief Class that manages window and its renderer.
class Renderer
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int window_width, window_height;

    WindowMode window_mode = FULLSCREEN_SIMPLE;

    void InitVideo(const char* path_to_icon = nullptr);

public:
    /// @brief Init video and create Renderer instance.
    /// Starts at simple fullscreen, can be changed later calling ChangeRes().
    /// @param path_to_icon path to the icon that will be set to program instance.
    /// Leave nullptr if you're setting icon in CMake.
    Renderer(const char* path_to_icon = nullptr);
    ~Renderer();

    SDL_Renderer* GetRenderer();
    SDL_Window* GetWindow();

    /// @brief Change game resolution and go/exit fullscreen if need.
    /// @param mode defines game window mode (see enum WindowMode).
    /// @param width desired width of the window.
    /// If you're going to fullscreen and leave 0, it considered as width of the screen.
    /// @param height desired height of the window.
    /// If you're going to fullscreen and leave 0, it considered as height of the screen.
    /// @param fullscreen_scale if go fullscreen, how much we should scale game to screen?
    /// If 1 - don't scale (pixel to pixel). If 2 - each 2x2 pixels sector in the screen
    /// will represent 1 pixel from the game.
    void ChangeRes(WindowMode mode = FULLSCREEN_SIMPLE, int width = 0, int height = 0, int fullscreen_scale = 1);

    WindowMode GetWindowMode();

    int GetWidth();
    int GetHeight();
};
