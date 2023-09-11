#pragma once

#include "../PObjs/BasePObj.h"

#include "../System/Controls.h"
#include "../System/Level.h"
#include "../System/Network.h"
#include "../System/Renderer.h"
#include "../System/Animations.h"

#include <box2d/box2d.h>

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>
#include <chrono>

/// @brief Class for managing either SDL2 window, renderer and Box2D world with physics objects within it.
class WorldManager
{
private:
    /// @brief Main Box2D world.
    b2World* world = nullptr;

    /// @brief Array of physics objects of the world. BasePObj is an abstract class and this array
    /// should only contain realizations of it (for example, PObjBox).
    std::vector<BasePObj*> objects;

    /// @brief Array of "ordered" physics objects that should be added to main ph. obj-s array when
    /// no world calculations are performing.
    std::vector<BasePObj*> order;

    /// @brief Array of loaded textures in "TEXTURE_PATH:TEXTURE" format.
    /// PObjs will manage it by themselves when Register() is called.
    std::map<std::string, SDL_Texture*> textures = std::map<std::string, SDL_Texture*>();

    // X offset of the camera in pixels (more - everything moves to left, less - to right).
    float x_offset = 0;
    // Y offset of the camera in pixels (more - everything moves down, less - up).
    float y_offset = 0;
    // Value that represents how to convert Box2D meters to screen pixels.
    // If rectangle of 2;3 meters should be showed at the screen,
    // with zoom 80 it will have a size of 2x80;3x80 = 160;240 pixels.
    float zoom = 80;
    // Maximum speed of camera movement by player.
    float move_speed;
    // Maximum speed of camera zoom by player.
    float zoom_speed;

    // How much frames Sand-Box2D remembers to calculate after-release camera inertia movement.
    int moving_inertia_frames;

    // Special value that sets up Box2D physics quality parameter (more - heavier but more precise).
    int physics_quality;

    // Path to the image used to represent "NO TEXTURE".
    std::string path_to_def_texture;

    // Is debug menu shown now?
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

    void FreeMemory();

    /// @brief Load level (object of class Level filled with required fields).
    /// First, it will destroy current loaded level (if it exists), then it will load attached level.
    /// @param level Level to be loaded.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    void LoadLevel(Level level, Renderer* rr);

    /// @brief Add physics object realization into the BasePObj array.
    /// Caution: object doesn't get to "objects" array instantly, first, it gets to "order" array
    /// and only at the end of Step() it `realizes` and adds to "objects" array.
    /// @param obj link to the physics object realization to add.
    void AddObject(BasePObj* obj);

    /// @brief Delete chosen physics object from an array.
    /// @param index index of the physics object realization in array to delete.
    void DeleteObject(int index);

    /// @brief Perform step of the world and logic, read keys and process them.
    /// @param rr link to renderer object (not link to SDL_Renderer) where to render.
    /// @param ctrl buttons pressed in this frame.
    /// @param old_ctrl buttons pressed in previous frame.
    /// @return true if WM wants to render next frame. False if it's the end and we need to load MainMenu instead.
    bool Step(Renderer* rr, Controls ctrl, Controls old_ctrl);

    /// @brief Render all the physics objects and show them.
    void Render(Renderer* rr, Controls ctrl);

    /// @brief Render debug screen at upper-left corner of a window.
    /// @param debugStrings information to be shown.
    void RenderDebugScreen(std::vector<std::string> debugStrings, Renderer* rr);
};
