#include "Controls.h"

bool Ctrl::isExit = false;
bool Ctrl::isReset = false;

SDL_Event Ctrl::e;

#ifdef Vita
void Ctrl::Check()
{
    SceCtrlData ctrl;
    sceCtrlPeekBufferPositive(0, &ctrl, 1);

    Ctrl::isReset = ctrl.buttons & SCE_CTRL_CROSS;
}
#else
void Ctrl::Check()
{
    while(SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            Ctrl::isExit = true;
        else if (e.key.keysym.sym == SDLK_r)
            Ctrl::isReset = e.type == SDL_KEYDOWN; // If keydown - reset continues (true), if keyup (!keydown) - reset stops (false)
    }
}
#endif

bool Ctrl::getIsExit()
{
    return Ctrl::isExit;
}
bool Ctrl::getIsReset()
{
    return Ctrl::isReset;
}
