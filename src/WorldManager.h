#pragma once

#include "PObjs/BasePObj.h"

#include "Controls.h"
#include "FontManager.h"
#include "Level.h"
#include "Network.h"
#include "Renderer.h"

#include <box2d/box2d.h>
#include <vector>
#include <iostream>
#include <algorithm>

/// @brief Class for managing either SDL2 window, renderer and Box2D world with physics objects within it.
class WorldManager
{
private:
    /// @brief Main Box2D world.
    b2World* world;

    // Game Renderer manager.
    Renderer renderer;
    
    /// Array of physics objects of the world. BasePObj is an abstract class and this array
    /// should only contain realizations of it (for example, PObjBox).
    std::vector<BasePObj*> objects;

    /// Array of "ordered" physics objects that should be added to main ph. obj-s array when
    /// no world calculations are performing.
    std::vector<BasePObj*> order;

    /// @brief Is WorldManager should adjust speed/FPS? Should be used only when there's need in this.
    bool speedCorrection;

    unsigned int a, b = 0;
    double delta = 0;

    float x_offset = 0, y_offset = 0, zoom = 80;
    float move_speed, zoom_speed;

    bool isDebug = false;

    /// @brief Current loaded level.
    Level level = Level();

    /// @brief Array with size of quantity of cycles which contains
    /// amount of frames before each cycle will do one step (every frame all the values here are
    /// decreasing, if some of they reach 0, corresponding cycle will do step and that value updates).
    std::vector<int> cyclesDelays = std::vector<int>();

    /// @brief This function will correct offset for X and Y when zooming to make 
    /// it look like zoom around mouse. Should be called strictly before changing zoom.
    /// @param mouse current mouse position.
    /// @param zoom_change how much zoom should change in the next frame?
    void CorrectOffset(SDL_Point mouse, float zoom_change);

public:
    /// @brief Init video, Box2D world and create WorldManager instance.
    /// @param path_to_font filepath to the font (*.ttf) file. If leave nullptr, WM wouldn't render any text.
    /// @param fpsCorrection is WorldManager should adjust speed/FPS? Should be used only when there's need in this.
    /// @param path_to_icon path to the icon that WM will set to program instance. Leave nullptr if you're setting icon in CMake.
    /// @param move_speed amount of pixels added to camera offset variable in one frame when pressed relevant button.
    /// @param zoom_speed amount of coefficiency multiplied to camera zoom variable in one frame when pressed relevant button.
    WorldManager(const char* path_to_font = nullptr, bool fpsCorrection = false, const char* path_to_icon = nullptr, float move_speed = 10, float zoom_speed = 0.03);
    ~WorldManager();

    /// @brief Add physics object realization into the BasePObj array.
    /// @param obj link to the physics object realization to add.
    void AddObject(BasePObj* obj);

    /// @brief Delete chosen physics object from an array.
    /// @param index index of the physics object realization in array to delete.
    void DeleteObject(int index);

    /// @brief Perform step of the world and logic, read keys and process them.
    /// @return running status (true - still running; false - stop).
    bool Step();
    
    /// @brief Render all the physics objects and show them.
    void Render();

    /// @brief Run main cycle of the program, exit only by initiative of user.
    void Cycle();

    /// @brief Load level (object of class Level filled with required fields).
    /// First, it will destroy current loaded level (if it exists), then it will load attached level.
    /// @param level Level to be loaded.
    void LoadLevel(Level level);

    /// @brief Render debug screen at upper-left corner of a window.
    /// @param debugStrings information to be shown.
    void RenderDebugScreen(std::vector<std::string> debugStrings);

    /// @brief Get rendering context.
    /// IMPORTANT: Gonna be deprecated soon (whole project architecture is going to change).
    /// @return link to the rendering context.
    SDL_Renderer* GetRenderer();
};
