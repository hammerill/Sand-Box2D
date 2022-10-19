#include "Controls.h"

bool Ctrl::exit = false;
bool Ctrl::reset = false;
bool Ctrl::fullscreen = false;
bool Ctrl::debug = false;
bool Ctrl::deleteObjs = false;

double Ctrl::moveUp = 0;
double Ctrl::moveRight = 0;
double Ctrl::moveDown = 0;
double Ctrl::moveLeft = 0;

double Ctrl::zoomIn = 0;
double Ctrl::zoomOut = 0;

bool Ctrl::moving = false;
int Ctrl::deltaX = 0;
int Ctrl::deltaY = 0;

bool Ctrl::isWheel = false;

SDL_Point Ctrl::mouse;

SDL_Event Ctrl::e;

#ifdef Vita
const double stickDeadZone = 10;
const double stickCenter = 128;

SceCtrlData ctrl;
SceTouchData touchxy[SCE_TOUCH_PORT_MAX_NUM];

SDL_Point old_mouse;

Ctrl::Ctrl()
{
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

    sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
    sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);
}

void Ctrl::Check()
{
    sceCtrlPeekBufferPositive(0, &ctrl, 1);
    sceTouchPeek(0, &touchxy[0], 1);

    Ctrl::reset = ctrl.buttons & SCE_CTRL_CROSS;
    Ctrl::debug = ctrl.buttons & SCE_CTRL_TRIANGLE;
    Ctrl::deleteObjs = ctrl.buttons & SCE_CTRL_CIRCLE;

    if (touchxy[0].reportNum == 1)
    {
        Ctrl::mouse = SDL_Point {touchxy[0].report[0].x / 2, touchxy[0].report[0].y / 2};

        if (Ctrl::moving) 
        {
            Ctrl::deltaX = Ctrl::mouse.x - old_mouse.x;
            Ctrl::deltaY = Ctrl::mouse.y - old_mouse.y;

            old_mouse = Ctrl::mouse;
        }
        else
        {
            old_mouse = Ctrl::mouse;

            Ctrl::deltaX = 0;
            Ctrl::deltaY = 0;
        }

        Ctrl::moving = true;
    }
    else
    {
        const int touchVel = 7;

        Ctrl::deltaX -= Ctrl::deltaX / touchVel;
        Ctrl::deltaY -= Ctrl::deltaY / touchVel;

        if (    Ctrl::deltaX <= touchVel && Ctrl::deltaY <= touchVel
            &&  Ctrl::deltaX >=-touchVel && Ctrl::deltaY >=-touchVel)
        {
            Ctrl::moving = false;

            Ctrl::deltaX = 0;
            Ctrl::deltaY = 0;
        }
    }
    

    if (ctrl.buttons & SCE_CTRL_UP) /////////////////////////////////// UP
        Ctrl::moveUp = 1;
    else if (ctrl.ly < stickCenter - stickDeadZone)
        Ctrl::moveUp = (stickCenter - ctrl.ly) / stickCenter;
    else
        Ctrl::moveUp = 0;
    
    if (ctrl.buttons & SCE_CTRL_RIGHT) /////////////////////////////////// RIGHT
        Ctrl::moveRight = 1;
    else if (ctrl.lx > stickCenter + stickDeadZone)
        Ctrl::moveRight = (ctrl.lx - stickCenter) / stickCenter;
    else
        Ctrl::moveRight = 0;

    if (ctrl.buttons & SCE_CTRL_DOWN) /////////////////////////////////// DOWN
        Ctrl::moveDown = 1;
    else if (ctrl.ly > stickCenter + stickDeadZone)
        Ctrl::moveDown = (ctrl.ly - stickCenter) / stickCenter;
    else
        Ctrl::moveDown = 0;
    
    if (ctrl.buttons & SCE_CTRL_LEFT) /////////////////////////////////// LEFT
        Ctrl::moveLeft = 1;
    else if (ctrl.lx < stickCenter - stickDeadZone)
        Ctrl::moveLeft = (stickCenter - ctrl.lx) / stickCenter;
    else
        Ctrl::moveLeft = 0;


    if (ctrl.buttons & SCE_CTRL_RTRIGGER) /////////////////////////////////// ZOOMIN
        Ctrl::zoomIn = 1;
    else if (ctrl.ry > stickCenter + stickDeadZone)
        Ctrl::zoomIn = (ctrl.ry - stickCenter) / stickCenter;
    else
        Ctrl::zoomIn = 0;

    if (ctrl.buttons & SCE_CTRL_LTRIGGER) /////////////////////////////////// ZOOMOUT
        Ctrl::zoomOut = 1;
    else if (ctrl.ry < stickCenter - stickDeadZone)
        Ctrl::zoomOut = (stickCenter - ctrl.ry) / stickCenter;
    else
        Ctrl::zoomOut = 0;
}
#else
void Ctrl::Check()
{
    Ctrl::deltaX = 0; 
    Ctrl::deltaY = 0;

    if (Ctrl::isWheel)
    {
        Ctrl::zoomIn = 0; 
        Ctrl::zoomOut = 0;
    }

    Ctrl::fullscreen = false;

    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            Ctrl::exit = true;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                Ctrl::moving = true;
                
                if (e.button.clicks == 2)
                    Ctrl::fullscreen = true;
            }
            
            break;
        case SDL_MOUSEBUTTONUP:
            Ctrl::moving = false;
            break;
        case SDL_MOUSEMOTION:
            Ctrl::deltaX = e.motion.xrel;
            Ctrl::deltaY = e.motion.yrel;
            
            Ctrl::mouse = SDL_Point {e.motion.x, e.motion.y};
            break;

        case SDL_MOUSEWHEEL:
            if (SDL_MOUSEWHEEL_NORMAL)
            {
                if (e.wheel.preciseY >= 0)
                    Ctrl::zoomIn = abs(e.wheel.preciseY);
                else
                    Ctrl::zoomOut = abs(e.wheel.preciseY);
            }
            else
            {
                if (e.wheel.preciseY >= 0)
                    Ctrl::zoomOut = abs(e.wheel.preciseY);
                else
                    Ctrl::zoomIn = abs(e.wheel.preciseY);
            }
            
            Ctrl::isWheel = true;
            break;

        case SDL_KEYDOWN: case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case SDLK_r:
                Ctrl::reset = e.type == SDL_KEYDOWN; // If keydown - reset continues (true), if keyup (!keydown) - reset stops (false)
                break;
            case SDLK_f:
                Ctrl::fullscreen = e.type == SDL_KEYDOWN; 
                break;
            case SDLK_TAB:
                Ctrl::debug = e.type == SDL_KEYDOWN; 
                break;
            case SDLK_x:
                Ctrl::deleteObjs = e.type == SDL_KEYDOWN; 
                break;

            case SDLK_UP: case SDLK_w:
                Ctrl::moveUp = e.type == SDL_KEYDOWN ? 1 : 0;
                break;
            case SDLK_RIGHT: case SDLK_d:
                Ctrl::moveRight = e.type == SDL_KEYDOWN ? 1 : 0;
                break;
            case SDLK_DOWN: case SDLK_s:
                Ctrl::moveDown = e.type == SDL_KEYDOWN ? 1 : 0;
                break;
            case SDLK_LEFT: case SDLK_a:
                Ctrl::moveLeft = e.type == SDL_KEYDOWN ? 1 : 0;
                break;

            case SDLK_e:
                Ctrl::zoomIn = e.type == SDL_KEYDOWN ? 1 : 0;
                Ctrl::isWheel = false;
                break;
            case SDLK_q:
                Ctrl::zoomOut = e.type == SDL_KEYDOWN ? 1 : 0;
                Ctrl::isWheel = false;
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

bool Ctrl::getExit()        { return Ctrl::exit; }
bool Ctrl::getReset()       { return Ctrl::reset; }
bool Ctrl::getFullscreen()  { return Ctrl::fullscreen; }
bool Ctrl::getDebug()       { return Ctrl::debug; }
bool Ctrl::getDeleteObjs()  { return Ctrl::deleteObjs; }

double Ctrl::getMoveUp()     { return Ctrl::moveUp; }
double Ctrl::getMoveRight()  { return Ctrl::moveRight; }
double Ctrl::getMoveDown()   { return Ctrl::moveDown; }
double Ctrl::getMoveLeft()   { return Ctrl::moveLeft; }

double Ctrl::getZoomIn()   { return Ctrl::zoomIn; }
double Ctrl::getZoomOut()  { return Ctrl::zoomOut; }

bool Ctrl::getMoving()  { return Ctrl::moving; }
int Ctrl::getDeltaX()   { return Ctrl::deltaX; }
int Ctrl::getDeltaY()   { return Ctrl::deltaY; }

bool Ctrl::getIsWheel()   { return Ctrl::isWheel; }

SDL_Point Ctrl::getMouse()  { return Ctrl::mouse; }

// OldCtrl::Copy(Ctrl* ctrl)
// {
//     OldCtrl::exit = ctrl->exit;
//     OldCtrl::reset = ctrl->reset;
//     OldCtrl::fullscreen = ctrl->fullscreen;
//     OldCtrl::debug = ctrl->debug;
//     OldCtrl::deleteObjs = ctrl->deleteObjs;

//     OldCtrl::moveUp = ctrl->moveUp;
//     OldCtrl::moveRight = ctrl->moveRight;
//     OldCtrl::moveDown = ctrl->moveDown;
//     OldCtrl::moveLeft = ctrl->moveLeft;

//     OldCtrl::zoomIn = ctrl->zoomIn;
//     OldCtrl::zoomOut = ctrl->zoomOut;

//     OldCtrl::moving = ctrl->moving;
//     OldCtrl::deltaX = ctrl->deltaX;
//     OldCtrl::deltaY = ctrl->deltaY;

//     OldCtrl::isWheel = ctrl->isWheel;

//     OldCtrl::mouse = ctrl->mouse;
// }
