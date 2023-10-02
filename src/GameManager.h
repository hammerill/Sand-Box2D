#pragma once

#ifdef Python_Test
#include "System/Python.h"
#endif

#include "System/Renderer.h"
#include "System/Controls.h"
#include "System/Settings.h"

#include "Screens/MainMenu.h"
#include "Screens/WorldManager.h"
#include "Screens/LangSelector.h"

/// @brief Which single class is responsible for rendering the entire screen?
enum CurrentVisual
{
    MAIN_MENU_VISUAL = 0,           // MainMenu is used now.
    WORLD_MANAGER_VISUAL,           // WorldManager is used now.
    LANG_SELECTOR_VISUAL            // LangSelector is used now.
};

/// @brief Class that manages everything.
class GameManager
{
private:
    // Main game renderer (not SDL object but my own class).
    Renderer* rr;
    // Object that shows what buttons are pressed now.
    Controls ctrl;
    // Object that shows what buttons were pressed in the previous frame.
    Controls old_ctrl;
    // Object that interrogates the game settings file.
    Settings settings;

    // MainMenu object. Look up CurrentVisual.
    MainMenu main_menu;
    // Link to the WorldManager. Look up CurrentVisual (btw it's a link cuz it's more complicated).
    WorldManager* world_manager;
    // LangSelector object. Look up CurrentVisual.
    LangSelector lang_selector;

    // Which single class is responsible for rendering the entire screen?
    CurrentVisual current_visual;

    // Default config for the fullscreen mode.
    WindowParams fullscreen = { FULLSCREEN_SIMPLE, 0, 0, 2 };
    // Default config for the windowed mode.
    WindowParams windowed = { WINDOWED, 960, 544 };

    // Used in order to use frame correcting.
    uint64_t a, b = 0;
    // Used in order to use frame correcting.
    double delta = 0;

    // At which frame mouse moved the last time? Used to hide it when inactive.
    uint64_t mouse_last_frame_move = 0;

public:
    /// @brief Init video and enter main menu.
    /// @param path_to_settings path to the JSON where user settings are stored.
    /// If doesn't exist - it'll create it. If leave nullptr, settings affect only current session.
    /// @param path_to_def_settings path to the JSON where default settings are stored.
    /// If leave nullptr, all default settings will be considered as 0 (which is unstable).
    GameManager(const char* path_to_settings = nullptr, const char* path_to_def_settings = nullptr);

    /// @brief Perform one step and call step functions on all the objects. To render them call Render().
    /// @return true if need to perform next step (continue). False if need to exit the game.
    bool Step();

    /// @brief Call render functions on all the objects and then show them on the screen.
    void Render();

    /// @brief Launch main game cycle. Will exit from it when user want it.
    void Cycle();
};

