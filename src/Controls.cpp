#include "Controls.h"

bool Ctrl::isExit = false;
bool Ctrl::isReset = false;

bool Ctrl::up = false;
bool Ctrl::right = false;
bool Ctrl::down = false;
bool Ctrl::left = false;
bool Ctrl::plus = false;
bool Ctrl::minus = false;

SDL_Event Ctrl::e;

#ifdef Vita
void Ctrl::Check()
{
    SceCtrlData ctrl;
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

    Ctrl::isReset = ctrl.buttons & SCE_CTRL_CROSS;

    Ctrl::up = ctrl.buttons & SCE_CTRL_UP;
    Ctrl::right = ctrl.buttons & SCE_CTRL_RIGHT;
    Ctrl::down = ctrl.buttons & SCE_CTRL_DOWN;
    Ctrl::left = ctrl.buttons & SCE_CTRL_LEFT;
    Ctrl::plus = ctrl.buttons & SCE_CTRL_R;
    Ctrl::minus = ctrl.buttons & SCE_CTRL_L;
}
#else
void Ctrl::Check()
{
    while(SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            Ctrl::isExit = true;
        else 
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_r:
                Ctrl::isReset = e.type == SDL_KEYDOWN; // If keydown - reset continues (true), if keyup (!keydown) - reset stops (false)
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

bool Ctrl::getIsExit() { return Ctrl::isExit; }
bool Ctrl::getIsReset() { return Ctrl::isReset; }

bool Ctrl::getUp() { return Ctrl::up; }
bool Ctrl::getRight() { return Ctrl::right; }
bool Ctrl::getDown() { return Ctrl::down; }
bool Ctrl::getLeft() { return Ctrl::left; }
bool Ctrl::getPlus() { return Ctrl::plus; }
bool Ctrl::getMinus() { return Ctrl::minus; }
