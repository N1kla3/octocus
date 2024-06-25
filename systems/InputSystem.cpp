#include "InputSystem.h"
#include "SpaceComponents.h"
#include "raylib.h"

void InputSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<velocity>();

    view.each([](velocity& vel) 
    {
        if (IsKeyPressed(KEY_W))
        {
            vel.dx = 1;
        }
        else vel.dx = 0;
    });
}
