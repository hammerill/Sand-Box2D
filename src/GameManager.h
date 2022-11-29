#pragma once

#include "Renderer.h"
#include "Controls.h"
#include "Settings.h"

// #include "MainMenu.h"
#include "WorldManager.h"

/// @brief Class that manages everything.
class GameManager
{
private:
    Renderer* renderer;
    Controls ctrl, old_ctrl;
    Settings settings;

    // MainMenu main_menu;
    WorldManager* world_manager;

    WindowParams fullscreen = {};
    WindowParams windowed = {WINDOWED, 960, 544};

    unsigned int a, b = 0;
    double delta = 0;

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

    /// @brief Launch main game cycle. Exit from it when user want it.
    void Cycle();
};

