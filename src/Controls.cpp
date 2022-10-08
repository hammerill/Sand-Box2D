#include "Controls.h"

bool Ctrl::exit = false;
bool Ctrl::reset = false;

bool Ctrl::up = false;
bool Ctrl::right = false;
bool Ctrl::down = false;
bool Ctrl::left = false;
bool Ctrl::plus = false;
bool Ctrl::minus = false;

SDL_Event Ctrl::e;

#ifdef Vita
const int stickDeadZone = 50;
const int stickCenter = 128;

void Ctrl::Check()
{
    SceCtrlData ctrl;
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

    Ctrl::reset = ctrl.buttons & SCE_CTRL_CROSS;

    Ctrl::up = ctrl.buttons & SCE_CTRL_UP ||
    (ctrl.ly < stickCenter - stickDeadZone);
    Ctrl::right = ctrl.buttons & SCE_CTRL_RIGHT ||
    (ctrl.lx > stickCenter + stickDeadZone);
    Ctrl::down = ctrl.buttons & SCE_CTRL_DOWN ||
    (ctrl.ly > stickCenter + stickDeadZone);
    Ctrl::left = ctrl.buttons & SCE_CTRL_LEFT ||
    (ctrl.lx < stickCenter - stickDeadZone);

    Ctrl::plus = ctrl.buttons & SCE_CTRL_RTRIGGER;
    Ctrl::minus = ctrl.buttons & SCE_CTRL_LTRIGGER;
}
#else
void Ctrl::Check()
{
    while(SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            Ctrl::exit = true;
        else 
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_r:
                Ctrl::reset = e.type == SDL_KEYDOWN; // If keydown - reset continues (true), if keyup (!keydown) - reset stops (false)
                break;

            case SDLK_UP: case SDLK_w:
                Ctrl::up = e.type == SDL_KEYDOWN;
                break;
            case SDLK_RIGHT: case SDLK_d:
                Ctrl::right = e.type == SDL_KEYDOWN;
                break;
            case SDLK_DOWN: case SDLK_s:
                Ctrl::down = e.type == SDL_KEYDOWN;
                break;
            case SDLK_LEFT: case SDLK_a:
                Ctrl::left = e.type == SDL_KEYDOWN;
                break;
            case SDLK_e:
                Ctrl::plus = e.type == SDL_KEYDOWN;
                break;
            case SDLK_q:
                Ctrl::minus = e.type == SDL_KEYDOWN;
                break;

            default:
                break;
            }
        }
    }
}
#endif

bool Ctrl::getExit() { return Ctrl::exit; }
bool Ctrl::getReset() { return Ctrl::reset; }

bool Ctrl::getUp() { return Ctrl::up; }
bool Ctrl::getRight() { return Ctrl::right; }
bool Ctrl::getDown() { return Ctrl::down; }
bool Ctrl::getLeft() { return Ctrl::left; }

bool Ctrl::getPlus() { return Ctrl::plus; }
bool Ctrl::getMinus() { return Ctrl::minus; }
