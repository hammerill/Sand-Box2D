#include "Controls.h"

#ifdef Vita
const double stickDeadZone = 10;
const double stickCenter = 128;

SceCtrlData vita_ctrl;
SceTouchData touchxy[SCE_TOUCH_PORT_MAX_NUM];

struct TouchSchema
{
    bool move = false;
    bool pinch = false;
};
bool operator==(const TouchSchema& ls, const TouchSchema& rs)
{
    return ls.move == rs.move && ls.pinch == rs.pinch;
}

TouchSchema old, now;

Controls::Controls()
{
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
}
Controls::~Controls() {}

void Controls::Check()
{
    sceCtrlPeekBufferPositive(0, &vita_ctrl, 1);
    sceTouchPeek(0, &touchxy[0], 1);

    old = now; // It looks so artistically.
    now = { touchxy[0].reportNum == 1 || touchxy[0].reportNum == 2,
            touchxy[0].reportNum == 2};

    Controls::debug = vita_ctrl.buttons & SCE_CTRL_TRIANGLE;

    Controls::actionUp = vita_ctrl.buttons & SCE_CTRL_UP;
    Controls::actionRight = vita_ctrl.buttons & SCE_CTRL_RIGHT;
    Controls::actionDown = vita_ctrl.buttons & SCE_CTRL_DOWN;
    Controls::actionLeft = vita_ctrl.buttons & SCE_CTRL_LEFT;

    Controls::actionEnter = vita_ctrl.buttons & SCE_CTRL_CROSS;

    Controls::menuUp = vita_ctrl.buttons & SCE_CTRL_UP;
    Controls::menuRight = vita_ctrl.buttons & SCE_CTRL_RIGHT;
    Controls::menuDown = vita_ctrl.buttons & SCE_CTRL_DOWN;
    Controls::menuLeft = vita_ctrl.buttons & SCE_CTRL_LEFT;

    Controls::menuEnter = vita_ctrl.buttons & SCE_CTRL_CROSS;
    Controls::menuBack = vita_ctrl.buttons & SCE_CTRL_CIRCLE;

    Controls::pause = vita_ctrl.buttons & (SCE_CTRL_START | SCE_CTRL_SELECT);

    if (touchxy[0].reportNum == 1)
    {
        Controls::mouse = SDL_Point {touchxy[0].report[0].x / 2, touchxy[0].report[0].y / 2};
        Controls::menuMouse = true;

        if (old == now)
        {
            Controls::isMoving = true;
            Controls::isPinching = false;
        }
        else
        {
            Controls::isMoving = false;
            Controls::isPinching = false;
        }
    }
    else if (touchxy[0].reportNum == 2)
    {
        Controls::mouse = SDL_Point
        {
            ((touchxy[0].report[0].x / 2) + (touchxy[0].report[1].x / 2)) / 2,
            ((touchxy[0].report[0].y / 2) + (touchxy[0].report[1].y / 2)) / 2
        };
        Controls::menuMouse = true;

        // Formula to calculate distance between two points
        Controls::pinch =  sqrt(
                                    pow((touchxy[0].report[1].x / 2) - (touchxy[0].report[0].x / 2), 2)
                                    +
                                    pow((touchxy[0].report[1].y / 2) - (touchxy[0].report[0].y / 2), 2)
                                );

        if (old == now)
        {
            Controls::isMoving = true;
            Controls::isPinching = true;
        }
        else
        {
            Controls::isMoving = false;
            Controls::isPinching = false;
        }
    }
    else
    {
        Controls::isMoving = false;
        Controls::menuMouse = false;
        Controls::isPinching = false;
    }


    if (vita_ctrl.ly < stickCenter - stickDeadZone) /////////////////////////////////// MOVEUP
        Controls::moveUp = (stickCenter - vita_ctrl.ly) / stickCenter;
    else
        Controls::moveUp = 0;

    if (vita_ctrl.lx > stickCenter + stickDeadZone) /////////////////////////////////// MOVERIGHT
        Controls::moveRight = (vita_ctrl.lx - stickCenter) / stickCenter;
    else
        Controls::moveRight = 0;

    if (vita_ctrl.ly > stickCenter + stickDeadZone) /////////////////////////////////// MOVEDOWN
        Controls::moveDown = (vita_ctrl.ly - stickCenter) / stickCenter;
    else
        Controls::moveDown = 0;

    if (vita_ctrl.lx < stickCenter - stickDeadZone) /////////////////////////////////// MOVELEFT
        Controls::moveLeft = (stickCenter - vita_ctrl.lx) / stickCenter;
    else
        Controls::moveLeft = 0;


    if (vita_ctrl.buttons & SCE_CTRL_RTRIGGER) /////////////////////////////////// ZOOMIN
        Controls::zoomIn = 1;
    else if (vita_ctrl.ry > stickCenter + stickDeadZone)
        Controls::zoomIn = (vita_ctrl.ry - stickCenter) / stickCenter;
    else
        Controls::zoomIn = 0;

    if (vita_ctrl.buttons & SCE_CTRL_LTRIGGER) /////////////////////////////////// ZOOMOUT
        Controls::zoomOut = 1;
    else if (vita_ctrl.ry < stickCenter - stickDeadZone)
        Controls::zoomOut = (stickCenter - vita_ctrl.ry) / stickCenter;
    else
        Controls::zoomOut = 0;
}
#else
SDL_GameController* controller;
const int controller_deadzone = 8000;

// TODO: To be moved to imaginary tools module maybe
SDL_GameController* findController()
{
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            return SDL_GameControllerOpen(i);
            break;
        }
    }
    return nullptr;
}

Controls::Controls() {}
Controls::~Controls() {}

void Controls::Check()
{
    if (Controls::isWheel)
    {
        Controls::zoomIn = 0;
        Controls::zoomOut = 0;
    }

    int rs; // I HAD to init it here. Compiler is NOT happy when I don't do that.

    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            Controls::exit = true;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                Controls::isMoving = true;
                Controls::menuMouse = true;

                if (e.button.clicks == 2)
                    Controls::fullscreen = true;
            }

            break;
        case SDL_MOUSEBUTTONUP:
            Controls::isMoving = false;
            Controls::menuMouse = false;
            Controls::fullscreen = false;
            break;
        case SDL_MOUSEMOTION:
            Controls::mouse = SDL_Point {e.motion.x, e.motion.y};
            break;

        case SDL_MOUSEWHEEL:
            if (SDL_MOUSEWHEEL_NORMAL)
            {
                if (e.wheel.preciseY >= 0)
                    Controls::zoomIn = abs(e.wheel.preciseY);
                else
                    Controls::zoomOut = abs(e.wheel.preciseY);
            }
            else
            {
                if (e.wheel.preciseY >= 0)
                    Controls::zoomOut = abs(e.wheel.preciseY);
                else
                    Controls::zoomIn = abs(e.wheel.preciseY);
            }

            Controls::isWheel = true;
            break;

        // Hint: ? 1 : 0 at the end is a conversion to analog value.
        case SDL_KEYDOWN: case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case SDLK_f:
                Controls::fullscreen = e.type == SDL_KEYDOWN;
                break;
            case SDLK_TAB:
                Controls::debug = e.type == SDL_KEYDOWN;
                break;
            case SDLK_x: case SDLK_BACKSPACE:
                Controls::menuBack = e.type == SDL_KEYDOWN;
                break;

            case SDLK_UP:
                Controls::moveUp = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::menuUp = e.type == SDL_KEYDOWN;
                break;
            case SDLK_RIGHT:
                Controls::moveRight = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::menuRight = e.type == SDL_KEYDOWN;
                break;
            case SDLK_DOWN:
                Controls::moveDown = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::menuDown = e.type == SDL_KEYDOWN;
                break;
            case SDLK_LEFT:
                Controls::moveLeft = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::menuLeft = e.type == SDL_KEYDOWN;
                break;

            case SDLK_w:
                Controls::actionUp = e.type == SDL_KEYDOWN;
                Controls::menuUp = e.type == SDL_KEYDOWN;
                break;
            case SDLK_d:
                Controls::actionRight = e.type == SDL_KEYDOWN;
                Controls::menuRight = e.type == SDL_KEYDOWN;
                break;
            case SDLK_s:
                Controls::actionDown = e.type == SDL_KEYDOWN;
                Controls::menuDown = e.type == SDL_KEYDOWN;
                break;
            case SDLK_a:
                Controls::actionLeft = e.type == SDL_KEYDOWN;
                Controls::menuLeft = e.type == SDL_KEYDOWN;
                break;

            case SDLK_SPACE: case SDLK_r: case SDLK_RETURN:
                Controls::actionEnter = e.type == SDL_KEYDOWN;
                Controls::menuEnter = e.type == SDL_KEYDOWN;
                break;

            case SDLK_z:
                Controls::menuEnter = e.type == SDL_KEYDOWN;
                break;

            case SDLK_e:
                Controls::zoomIn = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::isWheel = false;
                break;
            case SDLK_q:
                Controls::zoomOut = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::isWheel = false;
                break;

            case SDLK_ESCAPE:
                Controls::pause = e.type == SDL_KEYDOWN;
                break;

            default:
                break;
            }
            break;
        
        // PC CONTROLLER

        case SDL_CONTROLLERDEVICEADDED:
            if (!controller)
                controller = SDL_GameControllerOpen(e.cdevice.which);
            
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            if (controller)
            {
                SDL_GameControllerClose(controller);
                controller = findController();
            }
            break;

        case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
            if (controller) {
                SDL_EventType c_down = SDL_CONTROLLERBUTTONDOWN;

                switch (e.cbutton.button)
                {
                case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                    Controls::fullscreen = e.type == c_down;
                    break;
                case SDL_CONTROLLER_BUTTON_Y:
                    Controls::debug = e.type == c_down;
                    break;
                case SDL_CONTROLLER_BUTTON_B:
                    Controls::menuBack = e.type == c_down;
                    break;
                
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    Controls::actionUp = e.type == c_down;
                    Controls::menuUp = e.type == c_down;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    Controls::actionDown = e.type == c_down;
                    Controls::menuDown = e.type == c_down;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    Controls::actionRight = e.type == c_down;
                    Controls::menuRight = e.type == c_down;
                    break;
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    Controls::actionLeft = e.type == c_down;
                    Controls::menuLeft = e.type == c_down;
                    break;
                case SDL_CONTROLLER_BUTTON_A:
                    Controls::actionEnter = e.type == c_down;
                    Controls::menuEnter = e.type == c_down;
                    break;

                case SDL_CONTROLLER_BUTTON_START:
                    Controls::pause = e.type == c_down;
                    break;
                }
            }
            break;

        case SDL_CONTROLLERAXISMOTION:
            rs = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

            if (abs(rs) > SDL_JOYSTICK_AXIS_MAX / 5)
            {
                if (rs > 0)
                    Controls::zoomIn = (float)rs / SDL_JOYSTICK_AXIS_MAX;
                else
                    Controls::zoomOut = (float)rs / -SDL_JOYSTICK_AXIS_MAX;
            }
            else
            { 
                Controls::zoomIn = (float)SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / SDL_JOYSTICK_AXIS_MAX;
                Controls::zoomOut = (float)SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / SDL_JOYSTICK_AXIS_MAX;
            }

            break;

        ////////////////

        default:
            break;
        }
    }
}
#endif

bool Controls::Exit()           { return Controls::exit; }
bool Controls::Fullscreen()     { return Controls::fullscreen; }
bool Controls::Debug()          { return Controls::debug; }

double Controls::MoveUp()       { return Controls::moveUp; }
double Controls::MoveRight()    { return Controls::moveRight; }
double Controls::MoveDown()     { return Controls::moveDown; }
double Controls::MoveLeft()     { return Controls::moveLeft; }

double Controls::ZoomIn()       { return Controls::zoomIn; }
double Controls::ZoomOut()      { return Controls::zoomOut; }

bool Controls::IsMoving()       { return Controls::isMoving; }
bool Controls::MenuMouse()      { return Controls::menuMouse; }
SDL_Point Controls::GetMouse()  { return Controls::mouse; }

bool Controls::IsPinching()     { return Controls::isPinching; }
int Controls::GetPinch()        { return Controls::pinch; }

bool Controls::IsWheel()        { return Controls::isWheel; }

bool Controls::ActionUp()       { return Controls::actionUp; }
bool Controls::ActionRight()    { return Controls::actionRight; }
bool Controls::ActionDown()     { return Controls::actionDown; }
bool Controls::ActionLeft()     { return Controls::actionLeft; }

bool Controls::ActionEnter()    { return Controls::actionEnter; }

bool Controls::MenuUp()         { return Controls::menuUp; }
bool Controls::MenuRight()      { return Controls::menuRight; }
bool Controls::MenuDown()       { return Controls::menuDown; }
bool Controls::MenuLeft()       { return Controls::menuLeft; }

bool Controls::MenuEnter()      { return Controls::menuEnter; }
bool Controls::MenuBack()       { return Controls::menuBack; }

bool Controls::Pause()          { return Controls::pause; }

int Controls::GetControlTest()  { return Controls::control_test; }
