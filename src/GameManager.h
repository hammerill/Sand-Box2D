#pragma once

#include "System/Renderer.h"
#include "System/Controls.h"
#include "System/Settings.h"

#include "Screens/MainMenu.h"
#include "Screens/WorldManager.h"
#include "Screens/LangSelector.h"

#ifdef Vita
#define PYTHON_TEST 0
#if PYTHON_TEST
#include <python2.7/Python.h>
#endif
#endif

/// @brief Which single class is responsible for the entire screen?
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
    Renderer* rr;
    Controls ctrl, old_ctrl;
    Settings settings;

    MainMenu main_menu;
    WorldManager* world_manager;
    LangSelector lang_selector;

    CurrentVisual current_visual;

    WindowParams fullscreen = {FULLSCREEN_SIMPLE, 0, 0, 2};
    WindowParams windowed = {WINDOWED, 960, 544};

    uint64_t a, b = 0;
    double delta = 0;

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

