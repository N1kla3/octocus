#include "game.h"

int main()
{
    Game* game = new Game{};

    try
    {
        game->init();
        game->run();
    }
    catch (const char* str)
    {
        delete game;
    }
    delete game;
}
