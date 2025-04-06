#include "game.h"
#include "networking/Server.h"

void StartServer()
{
    // Begin server test
    // Create client and server sockets
    InitSteamDatagramConnectionSockets();
    // LocalUserInput_Init();

    ChatServer chat_server;
    chat_server.Run(33333);

    ShutdownSteamDatagramConnectionSockets();

    // End
}

int main()
{
    Game* game = new Game{};

    std::thread server(&StartServer);

    try
    {
        game->init();
        game->run();
        server.join();
    }
    catch ([[maybe_unused]] const char* str)
    {
        delete game;
    }
    delete game;
}
