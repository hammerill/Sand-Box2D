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
    bool fullscreen = false;
    bool debug = false;

    double moveUp = 0;
    double moveRight = 0;
    double moveDown = 0;
    double moveLeft = 0;

    double zoomIn = 0;
    double zoomOut = 0;

    bool mousePress = false;
    SDL_Point mouse;

    bool isPinching = false;
    int pinch = 0;

    bool isWheel = false;

    bool actionUp = false;
    bool actionRight = false;
    bool actionDown = false;
    bool actionLeft = false;
    
    bool actionEnter = false;

    bool menuUp = false;
    bool menuRight = false;
    bool menuDown = false;
    bool menuLeft = false;
    
    bool menuEnter = false;
    bool menuBack = false;
    
    bool pause = false;

    SDL_Event e;
    

public:
    Controls();
    ~Controls();

    /// @brief Check currently pressed buttons and write their status to private
    /// variables which can be accessed through getters functions of this class.
    void Check();

    /// @brief Is user does want to close the program? Should be handled.
    bool Exit();
    /// @brief Is user does want to go/exit fullscreen mode?
    bool Fullscreen();
    /// @brief Is user does want to activate/deactivate debug mode?
    bool Debug();

    /// @brief Is user does want to move camera up?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double MoveUp();
    /// @brief Is user does want to move camera right?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double MoveRight();
    /// @brief Is user does want to move camera down?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double MoveDown();
    /// @brief Is user does want to move camera left?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and maximum at 1.
    double MoveLeft();
    
    /// @brief Is user does want to zoom in?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and unlimited maximum.
    double ZoomIn();
    /// @brief Is user does want to zoom out?
    /// @return 0 if user doesn't pressing this button. 1 if they clicked it like binary button (yes/no).
    /// Otherwise analog value with minimum at 0 and unlimited maximum.
    double ZoomOut();
    
    /// @brief Is user pressing left mouse button or touchscreen right now in order to move camera?
    bool MousePress();
    /// @brief Get current mouse (or touchscreen) position.
    /// @return SDL_Point object with fields X and Y.
    SDL_Point GetMouse();

    /// @brief Is user zooming screen using touchscreen right now?
    bool IsPinching();
    /// @brief Get pinch value. It's a distance between two fingers.
    /// @return amount of pixels of distance between two fingers.
    int GetPinch();

    /// @brief Is user changing zoom right now using mouse wheel?
    bool IsWheel();

    // ACTIONS
    bool ActionUp();
    bool ActionRight();
    bool ActionDown();
    bool ActionLeft();

    bool ActionEnter();
    //////////

    // MENU
    bool MenuUp();
    bool MenuRight();
    bool MenuDown();
    bool MenuLeft();

    bool MenuEnter();
    bool MenuBack();

    bool Pause();
    ///////
};
