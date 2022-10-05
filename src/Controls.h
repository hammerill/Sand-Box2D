#include <SDL2/SDL.h>

#ifdef Vita
#include <psp2/ctrl.h>
#endif

class Ctrl
{
private:
    static bool isExit;
    static bool isReset;

    static SDL_Event e;
public:
    Ctrl();
    ~Ctrl();

    static void Check();

    static bool getIsExit();
    static bool getIsReset();
};
