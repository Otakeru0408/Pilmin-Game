#include "Game.h"


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    
    Game* g = new Game();

    g->RunLoop();

    g->TerminateGame();
    return 0;
}

