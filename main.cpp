#include "game.h"

int main()
{
    Game game{};

    try
    {
        game.init();
        game.run();
    }
    catch (const char* str)
    {

    }
}
