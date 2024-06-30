#include "InputSystem.h"
#include "SpaceComponents.h"
#include "raylib.h"

void InputSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<velocity>();

    view.each([](velocity& vel) 
    {
        vel.dx = 0;
        vel.dy = 0;
        if (IsKeyDown(KEY_W))
        {
            vel.dy += -1;
        }
        if (IsKeyDown(KEY_S))
        {
            vel.dy += 1;
        }
        if (IsKeyDown(KEY_D))
        {
            vel.dx += 1;
        }
        if (IsKeyDown(KEY_A))
        {
            vel.dx += -1;
        }
    });
}
