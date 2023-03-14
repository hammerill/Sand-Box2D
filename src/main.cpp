#include "main.h"

int main(int argv, char** args)
{
    GameManager* gm = new GameManager(SETTINGS_FILE, DEF_SETTINGS_FILE);
    gm->Cycle();

    return 0;
}
