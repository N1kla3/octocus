#include "MovementSystem.h"
#include "../game.h"
#include "SpaceComponents.h"
#include "raylib.h"

void MovementSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<const velocity, position>();

    Rectangle&& rect{};
    rect.x = 0.f;
    rect.y = 0.f;
    rect.width = Game::m_ScreenWidth;
    rect.height = Game::m_ScreenHeight;

    view.each([rect](const velocity vel, position& pos) 
    {
        bool should_move = CheckCollisionCircleRec(Vector2{pos.x + vel.dx, pos.y + vel.dy}, 1.f, rect);
        if (should_move)
        {
            pos.x += vel.dx;
            pos.y += vel.dy;
        }
    });
}
