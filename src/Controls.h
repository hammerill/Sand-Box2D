#pragma once

#include <SDL2/SDL.h>

#ifdef Vita
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#endif

/// @brief Class for accessing pressed keys and/or buttons
class Controls
{
protected:
    bool exit = false;
    bool reset = false;
    bool fullscreen = false;
    bool debug = false;
    bool reloadLevel = false;

    double moveUp = 0;
    double moveRight = 0;
    double moveDown = 0;
    double moveLeft = 0;

    double zoomIn = 0;
    double zoomOut = 0;

    bool isMoving = false;
    SDL_Point mouse;

    bool isPinching = false;
    int pinch = 0;

    bool isWheel = false;

    bool actionUp = false;
    bool actionRight = false;
    bool actionDown = false;
    bool actionLeft = false;
    
    bool actionEnter = false;

    SDL_Event e;
    

public:
    Controls();
    ~Controls();

    /// @brief Check currently pressed buttons and write their status to private
    /// variables which can be accessed through getters functions of this class.
    void Check();

    /// @brief Is user does want to close the program? Should be handled.
    bool GetExit();
    /// @brief Is user does want to reset the position and/or angles of ph. objects?
    bool GetReset();
    /// @brief Is user does want to go/exit fullscreen mode?
    bool GetFullscreen();
    /// @brief Is user does want to activate/deactivate debug mode?
    bool GetDebug();
    /// @brief Is user does want to reload current level?
    bool GetReloadLevel();

    /// @brief Is user does want to move camera up?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double GetMoveUp();
    /// @brief Is user does want to move camera right?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double GetMoveRight();
    /// @brief Is user does want to move camera down?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double GetMoveDown();
    /// @brief Is user does want to move camera left?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double GetMoveLeft();
    
    /// @brief Is user does want to zoom in?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and unlimited maximum.
    double GetZoomIn();
    /// @brief Is user does want to zoom out?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and unlimited maximum.
    double GetZoomOut();
    
    /// @brief Is user pressing left mouse button or touchscreen right now in order to move camera?
    bool GetIsMoving();
    /// @brief Get current mouse (or touchscreen) position.
    /// @return SDL_Point object with fields X and Y.
    SDL_Point GetMouse();

    /// @brief Is user zooming screen using touchscreen right now?
    bool GetIsPinching();
    /// @brief Get pinch value. It's a distance between two fingers.
    /// @return amount of pixels of distance between two fingers.
    int GetPinch();

    /// @brief Is user changing zoom right now using mouse wheel?
    bool GetIsWheel();

    // ACTIONS
    bool GetActionUp();
    bool GetActionRight();
    bool GetActionDown();
    bool GetActionLeft();

    bool GetActionEnter();
    //////////
};
