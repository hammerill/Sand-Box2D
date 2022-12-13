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
#include <numeric>
#include <map>

/// @brief Class for managing either SDL2 window, renderer and Box2D world with physics objects within it.
class WorldManager
{
private:
    /// @brief Main Box2D world.
    b2World* world;
    
    /// Array of physics objects of the world. BasePObj is an abstract class and this array
    /// should only contain realizations of it (for example, PObjBox).
    std::vector<BasePObj*> objects;

    /// Array of "ordered" physics objects that should be added to main ph. obj-s array when
    /// no world calculations are performing.
    std::vector<BasePObj*> order;

    /// Array of loaded textures in "TEXTURE_PATH:TEXTURE" format.
    /// PObjs will manage it by themselves when Register() is called.
    std::map<std::string, SDL_Texture*> textures = std::map<std::string, SDL_Texture*>();

    float x_offset = 0, y_offset = 0, zoom = 80;
    float move_speed, zoom_speed;

    int moving_inertia_frames;

    int physics_quality;

    std::string path_to_def_texture;

    bool isDebug = false;

    /// @brief Current loaded level.
    Level level = Level();

    /// @brief Array with size of quantity of cycles which contains
    /// amount of frames before each cycle will do one step (every frame all the values here are
    /// decreasing, if some of they reach 0, corresponding cycle will do step and that value updates).
    std::vector<int> cyclesDelays = std::vector<int>();

    /// @brief List of actions to perform when clicked special buttons.
    Json::Value actions = Json::Value();

    /// @brief This function will correct offset for X and Y when zooming to make 
    /// it look like zoom around mouse. Should be called strictly before changing zoom.
    /// @param mouse current mouse position.
    /// @param zoom_change how much zoom should change in the next frame?
    void CorrectOffset(SDL_Point mouse, float zoom_change);

public:
    /// @brief Init Box2D world and create WorldManager instance.
    /// @param path_to_def_texture path to the default texture which loaded when no texture provided for the PObj.
    /// @param physics_quality higher value - more precise but slower calculation.
    /// @param moving_inertia_frames how much frames should WM remember when calculating past-move inertia?.
    /// @param move_speed amount of pixels added to camera offset variable in one frame when pressed relevant button.
    /// @param zoom_speed amount of coefficiency multiplied to camera zoom variable in one frame when pressed relevant button.
    WorldManager(   std::string path_to_def_texture,
                    int physics_quality = 16,
                    int moving_inertia_frames = 10,
                    float move_speed = 10,
                    float zoom_speed = 0.03);
    ~WorldManager();

    /// @brief Load level (object of class Level filled with required fields).
    /// First, it will destroy current loaded level (if it exists), then it will load attached level.
    /// @param level Level to be loaded.
    /// @param renderer link to renderer object (not link to SDL_Renderer) where to render.
    void LoadLevel(Level level, Renderer* renderer);

    /// @brief Add physics object realization into the BasePObj array.
    /// @param obj link to the physics object realization to add.
    void AddObject(BasePObj* obj);

    /// @brief Delete chosen physics object from an array.
    /// @param index index of the physics object realization in array to delete.
    void DeleteObject(int index);

    /// @brief Perform step of the world and logic, read keys and process them.
    /// @param renderer link to renderer object (not link to SDL_Renderer) where to render.
    /// @param ctrl buttons pressed in this frame.
    /// @param old_ctrl buttons pressed in previous frame.
    void Step(Renderer* renderer, Controls ctrl, Controls old_ctrl);
    
    /// @brief Render all the physics objects and show them.
    void Render(Renderer* renderer, Controls ctrl);

    /// @brief Render debug screen at upper-left corner of a window.
    /// @param debugStrings information to be shown.
    void RenderDebugScreen(std::vector<std::string> debugStrings, Renderer* renderer);
};
