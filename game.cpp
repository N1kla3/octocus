#include "game.h"
#include "raylib.h"
#include "components/SpaceComponents.h"
#include <chrono>

void Game::init()
{
    InitWindow(m_ScreenWidth, m_ScreenHeight, "Octocus");

    SetTargetFPS(120);
}

void Game::run()
{
    using namespace std::chrono;
    auto begin = steady_clock::now();

    while (!WindowShouldClose())
    {
        const auto current = steady_clock::now();
        float duration = std::chrono::duration<float>{current - begin}.count();

        update(duration);

        begin = current;
    }

    tearDown();
}

void Game::update(float delta)
{
    updateGameplay(delta);
    updateDrawFrame(delta);
}

void Game::tearDown()
{
    CloseWindow();
}

void Game::updateGameplay(float delta)
{
    auto view = m_Registry.view<const position, velocity>();

    // use a callback
    view.each([](const auto &pos, auto &vel) { /* ... */ });

    // use an extended callback
    view.each([](const auto entity, const auto &pos, auto &vel) { /* ... */ });

    // use a range-for
    for(auto [entity, pos, vel]: view.each()) {
        // ...
    }

    // use forward iterators and get only the components of interest
    for(auto entity: view) {
        auto &vel = view.get<velocity>(entity);
        // ...
    }
}

void Game::updateDrawFrame(float delta)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}
