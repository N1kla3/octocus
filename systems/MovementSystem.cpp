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

    view.each(
            [rect, delta](const velocity vel, position& pos)
            {
                float change_x = vel.dx * delta;
                float change_y = vel.dy * delta;
                bool should_move_x = CheckCollisionCircleRec(Vector2{pos.x + change_x, pos.y}, 1.f, rect);
                if (should_move_x)
                {
                    pos.x += change_x;
                }
                bool should_move_y = CheckCollisionCircleRec(Vector2{pos.x, pos.y + change_y}, 1.f, rect);
                if (should_move_y)
                {
                    pos.y += change_y;
                }
            });
}
