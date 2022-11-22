#include "Controls.h"

#ifdef Vita
const double stickDeadZone = 10;
const double stickCenter = 128;

SceCtrlData ctrl;
SceTouchData touchxy[SCE_TOUCH_PORT_MAX_NUM];

SDL_Point old_mouse;
int old_pinch;

bool isInPinchVelState = false;

Controls::Controls()
{
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
}
Controls::~Controls() {}

void Controls::Check()
{
    sceCtrlPeekBufferPositive(0, &ctrl, 1);
    sceTouchPeek(0, &touchxy[0], 1);

    Controls::reset = ctrl.buttons & SCE_CTRL_CROSS;
    Controls::debug = ctrl.buttons & SCE_CTRL_TRIANGLE;
    Controls::reloadLevel = ctrl.buttons & SCE_CTRL_CIRCLE;

    if (touchxy[0].reportNum == 1)
    {
        Controls::mouse = SDL_Point {touchxy[0].report[0].x / 2, touchxy[0].report[0].y / 2};

        if (Controls::isMoving && !Controls::isPinching && !isInPinchVelState) 
        {
            Controls::deltaX = Controls::mouse.x - old_mouse.x;
            Controls::deltaY = Controls::mouse.y - old_mouse.y;

            old_mouse = Controls::mouse;
        }
        else
        {
            old_mouse = Controls::mouse;

            Controls::deltaX = 0;
            Controls::deltaY = 0;
        }

        Controls::isMoving = true;
        Controls::isPinching = false;
        isInPinchVelState = false;
    }
    else if (touchxy[0].reportNum == 2)
    {
        Controls::mouse = SDL_Point
        {
            ((touchxy[0].report[0].x / 2) + (touchxy[0].report[1].x / 2)) / 2,
            ((touchxy[0].report[0].y / 2) + (touchxy[0].report[1].y / 2)) / 2
        };

        // Formula to calculate distance between two points
        Controls::deltaPinch =  sqrt(
                                    pow((touchxy[0].report[1].x / 2) - (touchxy[0].report[0].x / 2), 2)
                                    +
                                    pow((touchxy[0].report[1].y / 2) - (touchxy[0].report[0].y / 2), 2)
                                );

        if (Controls::isMoving && Controls::isPinching) 
        {
            Controls::deltaX = Controls::mouse.x - old_mouse.x;
            Controls::deltaY = Controls::mouse.y - old_mouse.y;

            Controls::deltaPinch -= old_pinch;

            old_mouse = Controls::mouse;
            old_pinch += Controls::deltaPinch;
        }
        else
        {
            old_mouse = Controls::mouse;
            old_pinch = Controls::deltaPinch;

            Controls::deltaX = 0;
            Controls::deltaY = 0;
            Controls::deltaPinch = 0;
        }

        Controls::isMoving = true;
        Controls::isPinching = true;
    }
    else
    {
        const int touchVel = 7;

        Controls::deltaX -= Controls::deltaX / touchVel;
        Controls::deltaY -= Controls::deltaY / touchVel;

        if (    Controls::deltaX <= touchVel && Controls::deltaY <= touchVel
            &&  Controls::deltaX >=-touchVel && Controls::deltaY >=-touchVel)
        {
            Controls::isMoving = false;
            isInPinchVelState = false;

            Controls::deltaX = 0;
            Controls::deltaY = 0;
        }
        else
            isInPinchVelState = true;

        Controls::isPinching = false;
    }
    

    if (ctrl.buttons & SCE_CTRL_UP) /////////////////////////////////// UP
        Controls::moveUp = 1;
    else if (ctrl.ly < stickCenter - stickDeadZone)
        Controls::moveUp = (stickCenter - ctrl.ly) / stickCenter;
    else
        Controls::moveUp = 0;
    
    if (ctrl.buttons & SCE_CTRL_RIGHT) /////////////////////////////////// RIGHT
        Controls::moveRight = 1;
    else if (ctrl.lx > stickCenter + stickDeadZone)
        Controls::moveRight = (ctrl.lx - stickCenter) / stickCenter;
    else
        Controls::moveRight = 0;

    if (ctrl.buttons & SCE_CTRL_DOWN) /////////////////////////////////// DOWN
        Controls::moveDown = 1;
    else if (ctrl.ly > stickCenter + stickDeadZone)
        Controls::moveDown = (ctrl.ly - stickCenter) / stickCenter;
    else
        Controls::moveDown = 0;
    
    if (ctrl.buttons & SCE_CTRL_LEFT) /////////////////////////////////// LEFT
        Controls::moveLeft = 1;
    else if (ctrl.lx < stickCenter - stickDeadZone)
        Controls::moveLeft = (stickCenter - ctrl.lx) / stickCenter;
    else
        Controls::moveLeft = 0;


    if (ctrl.buttons & SCE_CTRL_RTRIGGER) /////////////////////////////////// ZOOMIN
        Controls::zoomIn = 1;
    else if (ctrl.ry > stickCenter + stickDeadZone)
        Controls::zoomIn = (ctrl.ry - stickCenter) / stickCenter;
    else
        Controls::zoomIn = 0;

    if (ctrl.buttons & SCE_CTRL_LTRIGGER) /////////////////////////////////// ZOOMOUT
        Controls::zoomOut = 1;
    else if (ctrl.ry < stickCenter - stickDeadZone)
        Controls::zoomOut = (stickCenter - ctrl.ry) / stickCenter;
    else
        Controls::zoomOut = 0;
}
#else
Controls::Controls() {}
Controls::~Controls() {}

void Controls::Check()
{
    Controls::deltaX = 0; 
    Controls::deltaY = 0;

    if (Controls::isWheel)
    {
        Controls::zoomIn = 0; 
        Controls::zoomOut = 0;
    }

    Controls::fullscreen = false;

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
                
                if (e.button.clicks == 2)
                    Controls::fullscreen = true;
            }
            
            break;
        case SDL_MOUSEBUTTONUP:
            Controls::isMoving = false;
            break;
        case SDL_MOUSEMOTION:
            Controls::deltaX = e.motion.xrel;
            Controls::deltaY = e.motion.yrel;
            
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

        case SDL_KEYDOWN: case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case SDLK_r:
                Controls::reset = e.type == SDL_KEYDOWN; // If keydown - reset continues (true), if keyup (!keydown) - reset stops (false)
                break;
            case SDLK_f:
                Controls::fullscreen = e.type == SDL_KEYDOWN; 
                break;
            case SDLK_TAB:
                Controls::debug = e.type == SDL_KEYDOWN; 
                break;
            case SDLK_x:
                Controls::reloadLevel = e.type == SDL_KEYDOWN; 
                break;

            case SDLK_UP: case SDLK_w:
                Controls::moveUp = e.type == SDL_KEYDOWN ? 1 : 0;
                break;
            case SDLK_RIGHT: case SDLK_d:
                Controls::moveRight = e.type == SDL_KEYDOWN ? 1 : 0;
                break;
            case SDLK_DOWN: case SDLK_s:
                Controls::moveDown = e.type == SDL_KEYDOWN ? 1 : 0;
                break;
            case SDLK_LEFT: case SDLK_a:
                Controls::moveLeft = e.type == SDL_KEYDOWN ? 1 : 0;
                break;

            case SDLK_e:
                Controls::zoomIn = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::isWheel = false;
                break;
            case SDLK_q:
                Controls::zoomOut = e.type == SDL_KEYDOWN ? 1 : 0;
                Controls::isWheel = false;
                break;

            default:
                break;
            }
            break;
            
        default:
            break; // COMBO: QUADRO BREAKS
        }
    }
}
#endif

bool Controls::GetExit()        { return Controls::exit; }
bool Controls::GetReset()       { return Controls::reset; }
bool Controls::GetFullscreen()  { return Controls::fullscreen; }
bool Controls::GetDebug()       { return Controls::debug; }
bool Controls::GetReloadLevel() { return Controls::reloadLevel; }

double Controls::GetMoveUp()    { return Controls::moveUp; }
double Controls::GetMoveRight() { return Controls::moveRight; }
double Controls::GetMoveDown()  { return Controls::moveDown; }
double Controls::GetMoveLeft()  { return Controls::moveLeft; }

double Controls::GetZoomIn()    { return Controls::zoomIn; }
double Controls::GetZoomOut()   { return Controls::zoomOut; }

bool Controls::GetIsMoving()    { return Controls::isMoving; }
int Controls::GetDeltaX()       { return Controls::deltaX; }
int Controls::GetDeltaY()       { return Controls::deltaY; }

bool Controls::GetIsPinching()  { return Controls::isPinching; }
int Controls::GetDeltaPinch()   { return Controls::deltaPinch; }

bool Controls::GetIsWheel()     { return Controls::isWheel; }

SDL_Point Controls::GetMouse()  { return Controls::mouse; }
