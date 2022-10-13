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

float Ctrl::wheel = 0;

SDL_Point Ctrl::mouse;

SDL_Event Ctrl::e;

#ifdef Vita
const double stickDeadZone = 10;
const double stickCenter = 128;

void Ctrl::Check()
{
    SceCtrlData ctrl;
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

    Ctrl::reset = ctrl.buttons & SCE_CTRL_CROSS;
    Ctrl::debug = ctrl.buttons & SCE_CTRL_TRIANGLE;
    Ctrl::deleteObjs = ctrl.buttons & SCE_CTRL_CIRCLE;

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
    Ctrl::wheel = 0;

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
            break;

        case SDL_MOUSEWHEEL:
            Ctrl::wheel = e.wheel.preciseY;
            Ctrl::mouse = {e.button.x, e.button.y};
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
                break;
            case SDLK_q:
                Ctrl::zoomOut = e.type == SDL_KEYDOWN ? 1 : 0;
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

float Ctrl::getWheel()    { return Ctrl::wheel; }

SDL_Point Ctrl::getMouse()  { return Ctrl::mouse; }
