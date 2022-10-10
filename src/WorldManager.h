#pragma once

#include <box2d/box2d.h>
#include <vector>

#include "PhysicsObj.h"

/// @brief Class for managing either SDL2 window, renderer and Box2D world with physics objects within it.
class WorldManager
{
private:
    /// @brief Main Box2D world.
    b2World* world;

    /// @brief The rendering context.
    SDL_Renderer* renderer;
    
    /// Array of physics objects of the world. PhysicsObj is an abstract class and this array
    /// should only contain realizations of it (for example, BoxEntity).
    std::vector<PhysicsObj*> objects;

    /// Array of "ordered" physics objects that should be added to main ph. obj-s array when
    /// no world calculations are performing.
    std::vector<PhysicsObj*> order;

    int WINDOW_WIDTH, WINDOW_HEIGHT;

    /// @brief Is WorldManager should adjust speed/FPS? Should be used only when there's need in this.
    bool speedCorrection;

    SDL_Window* window;

    void initVideo();

    unsigned int a, b = 0;
    double delta = 0;

    float x_offset = 0, y_offset = 0, zoom = 80;
    float move_speed, zoom_speed;

    bool holdingFullscreenButton = false;
    bool isFullscreen = false;

public:
    /// @brief Init video, Box2D world and create WorldManager instance.
    /// @param WINDOW_WIDTH resolution of the window width in pixels. If 0, width would be adjusted automatically.
    /// @param WINDOW_HEIGHT resolution of the window height in pixels. If 0, height would be adjusted automatically.
    /// @param fpsCorrection is WorldManager should adjust speed/FPS? Should be used only when there's need in this.
    /// @param move_speed amount of pixels added to camera offset variable in one frame when pressed relevant button.
    /// @param zoom_speed amount of coefficiency added to camera zoom variable in one frame when pressed relevant button.
    WorldManager(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool fpsCorrection = false, float move_speed = 10, float zoom_speed = 1);
    ~WorldManager();

    /// @brief Add physics object realization into the PhysicsObj array.
    /// @param obj link to the physics object realization to add.
    void addObject(PhysicsObj* obj);

    /// @brief Delete chosen physics object from an array.
    /// @param index index of the physics object realization in array to delete.
    void deleteObject(int index);

    /// @brief Perform step of the world and logic, read keys and process them.
    /// @return running status (true - still running; false - stop).
    bool Step();
    
    /// @brief Render all the physics objects and show them.
    void Render();

    /// @brief Run main cycle of the program, exit only by initiative of user.
    void Cycle();
};
