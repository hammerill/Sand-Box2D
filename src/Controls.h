#pragma once

#include <SDL2/SDL.h>

#ifdef Vita
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#endif

/// @brief Static class for accessing pressed keys and/or buttons
class Ctrl
{
protected:
    static bool exit;
    static bool reset;
    static bool fullscreen;
    static bool debug;
    static bool reloadLevel;

    static double moveUp;
    static double moveRight;
    static double moveDown;
    static double moveLeft;

    static double zoomIn;
    static double zoomOut;

    static bool isMoving;
    static int deltaX;
    static int deltaY;

    static bool isPinching;
    static int deltaPinch;

    static bool isWheel;

    static SDL_Point mouse;

    static SDL_Event e;

public:
    Ctrl();
    ~Ctrl();

    /// @brief Check currently pressed buttons and write their status to private
    /// variables which can be accessed through getters functions of this class.
    static void Check();

    /// @brief Is user does want to close the program? Should be handled.
    static bool getExit();
    /// @brief Is user does want to reset the position and/or angles of ph. objects?
    static bool getReset();
    /// @brief Is user does want to go/exit fullscreen mode?
    static bool getFullscreen();
    /// @brief Is user does want to activate/deactivate debug mode?
    static bool getDebug();
    /// @brief Is user does want to reload current level?
    static bool getReloadLevel();

    /// @brief Is user does want to move camera up?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    static double getMoveUp();
    /// @brief Is user does want to move camera right?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    static double getMoveRight();
    /// @brief Is user does want to move camera down?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    static double getMoveDown();
    /// @brief Is user does want to move camera left?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    static double getMoveLeft();
    
    /// @brief Is user does want to zoom in?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and unlimited maximum.
    static double getZoomIn();
    /// @brief Is user does want to zoom out?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and unlimited maximum.
    static double getZoomOut();
    
    /// @brief Is user pressing left mouse button or touchscreen right now in order to move camera?
    static bool getIsMoving();
    
    /// @brief Get delta X.
    /// @return amount of pixels that user moved with mouse (or swiped) on X relatively to previous frame.
    /// @example Users mouse speed 3 pixels/frame. This function will return 3 if they also move mouse 
    /// strictly horizontally right or -3 if strictly horizontally left.
    static int getDeltaX();
    /// @brief Get delta Y.
    /// @return amount of pixels that user moved with mouse (or swiped) on Y relatively to previous frame.
    /// @example Users mouse speed 3 pixels/frame. This function will return 3 if they also move mouse 
    /// strictly vertically down or -3 if strictly vertically up.
    static int getDeltaY();

    /// @brief Is user zooming screen using touchscreen right now?
    static bool getIsPinching();
    
    /// @brief Get delta of zoom. Caution, this function is pixel-specific, and so should be properly
    /// handled at the getting side.
    /// @return amount of pixels by how much user pinched screen relatively to previous frame.
    static int getDeltaPinch();

    /// @brief Is user changing zoom right now using mouse wheel?
    static bool getIsWheel();

    /// @brief Get current mouse (or touchscreen) position.
    /// @return SDL_Point object with fields X and Y.
    static SDL_Point getMouse();
};
