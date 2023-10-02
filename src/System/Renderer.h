#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Fonts.h"
#include "SoundManager.h"
#include "Controls.h"

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
    // Main game SDL window.
    SDL_Window* window;
    // Main game SDL renderer.
    SDL_Renderer* renderer;

    // Game resolution.
    int window_width, window_height;

    // Which window mode is used now? See enum WindowMode.
    WindowMode window_mode = FULLSCREEN_SIMPLE;

    // Where is window icon located? Used only in Linux.
    const char* path_to_icon;

    // Is cursor active now? If so, render it.
    bool cursor;

    // How much frames passed already? (will this variable overlap some day?)
    uint64_t frames = 0;

    // Link to to the main game font.
    Font* font = new Font();
    // Link to to the main game Japanese font.
    Font* font_jp = new Font();

    // Link to the SoundManager (my own class, go see it).
    SoundManager* sounds = new SoundManager();

public:
    ~Renderer();

    /// @brief Init video.
    /// @param params declare settings of your window here (see struct WindowParams).
    /// You can change them later using ChangeRes().
    /// @param path_to_sfx_base path to the directory where all the sound effects are located. Should
    /// contain index.json which looks like this: {"big_sound_name": "PathTo/Big/Sound.wav", "...": "..."}
    /// @param path_to_font path to the font (*.ttf) file which will be always rendered.
    /// Leave nullptr if you don't want to render any text.
    /// @param path_to_font_jp path to the font (*.ttf) file which will be used to render Japanese text.
    /// Leave nullptr if you don't want to render any text.
    /// @param path_to_icon path to the icon that will be set to program instance.
    /// Leave nullptr if you're setting icon in CMake.
    void InitVideo(
        WindowParams params,
        const char* path_to_sfx_base = nullptr,
        const char* path_to_font = nullptr,
        const char* path_to_font_jp = nullptr,
        const char* path_to_icon = nullptr);

    // Get main game SDL window.
    SDL_Renderer* GetRenderer();
    // Get main game SDL renderer.
    SDL_Window* GetWindow();
    // Get link to the SoundManager.
    SoundManager* GetSounds();
    /// @brief Get link to the Font object.
    /// @param jp do you want to load the Japanese font? If not, return the regular one.
    Font* GetFont(bool jp = false);
    // Get current WindowParams (you can look up that struct).
    WindowParams GetWindowParams();

    // Get game width.
    int GetWidth();
    // Get game height.
    int GetHeight();

    /// @brief Increment rendered frames count.
    void AddFrame();
    /// @brief Get rendered frames count.
    uint64_t GetFrames();

    /// @brief Change game resolution and go/exit fullscreen if need.
    /// @param params declare settings of your window here (see struct WindowParams).
    void ChangeRes(WindowParams params);

    /// @brief Enable or disable cursor render.
    void SetCursor(bool enable = true);
    /// @brief Is cursor showed now? If platform is Vita it's true when screen is being touched.
    bool GetCursor(Controls ctrl);

    /// @brief Render text. Will use internal Font object to do that
    /// (pass this function into it).
    /// @param text text to be rendered.
    /// @param x X position of the beginning of the text.
    /// @param y Y position of the beginning of the text.
    /// @param scale How much scale font? Also multiplied by const FontWidth.
    /// @param center Do we need to center text? If yes, given X and Y would be center of the text.
    /// @param jp Do we want to render text using Japanese font?
    /// @param r Color.
    /// @param g Color.
    /// @param b Color.
    void RenderText(const char* text, int x = 0, int y = 0, float scale = 1, bool center = false, bool jp = false, Uint8 r = 0xFF, Uint8 g = 0xFF, Uint8 b = 0xFF);
};
