#include "raylib.h"
#include <entt/entt.hpp>
#include "SpaceComponents.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame();

void update(entt::registry &registry) {
    auto view = registry.view<const position, velocity>();

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
}//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // ENTITY
    entt::registry registry;

    for(auto i = 0u; i < 10u; ++i) {
        const auto entity = registry.create();
        registry.emplace<position>(entity, i * 1.f, i * 1.f);
        if(i % 2 == 0) { registry.emplace<velocity>(entity, i * .1f, i * .1f); }
    }

    update(registry);
    // ENTITY

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
    CloseWindow();

    return 0;
}

void UpdateDrawFrame()
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}