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
    int deltaX = 0;
    int deltaY = 0;

    bool isPinching = false;
    int deltaPinch = 0;

    bool isWheel = false;

    SDL_Point mouse;

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
    
    /// @brief Get delta X.
    /// @return amount of pixels that user moved with mouse (or swiped) on X relatively to previous frame.
    /// @example Users mouse speed 3 pixels/frame. This function will return 3 if they also move mouse 
    /// strictly horizontally right or -3 if strictly horizontally left.
    int GetDeltaX();
    /// @brief Get delta Y.
    /// @return amount of pixels that user moved with mouse (or swiped) on Y relatively to previous frame.
    /// @example Users mouse speed 3 pixels/frame. This function will return 3 if they also move mouse 
    /// strictly vertically down or -3 if strictly vertically up.
    int GetDeltaY();

    /// @brief Is user zooming screen using touchscreen right now?
    bool GetIsPinching();
    
    /// @brief Get delta of zoom. Caution, this function is pixel-specific, and so should be properly
    /// handled at the getting side.
    /// @return amount of pixels by how much user pinched screen relatively to previous frame.
    int GetDeltaPinch();

    /// @brief Is user changing zoom right now using mouse wheel?
    bool GetIsWheel();

    /// @brief Get current mouse (or touchscreen) position.
    /// @return SDL_Point object with fields X and Y.
    SDL_Point GetMouse();
};
