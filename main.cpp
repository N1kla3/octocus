#include "game.h"
#include "networking/Server.h"

int main()
{
    Game* game = new Game{};

    // Begin server test
    // Create client and server sockets
    InitSteamDatagramConnectionSockets();
    LocalUserInput_Init();

    ChatServer chat_server;
    chat_server.Run(33333);

    ShutdownSteamDatagramConnectionSockets();

    // End

    try
    {
        game->init();
        game->run();
    }
    catch ([[maybe_unused]] const char* str)
    {
        delete game;
    }
    delete game;
}
