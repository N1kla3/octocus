#include "DrawSystem.h"
#include "../game.h"
#include "AnimationSystem.h"
#include "BorderComponent.h"
#include "LifeComponents.h"
#include "RenderComponent.h"
#include "SpaceComponents.h"
#include "raylib.h"

void DrawSystem::update(entt::registry& registry, const float delta, const GameStatus status)
{
    RenderComponent player_render{};
    constexpr float half_width = static_cast<float>(Game::m_ScreenWidth) / 2;
    constexpr float half_height = static_cast<float>(Game::m_ScreenHeight) / 2;
    const auto camera_view = registry.view<const Position, const CameraTarget, const RenderComponent>();
    for (const auto& [ent, pos, target, render]: camera_view.each())
    {
        Camera2D camera{};
        camera.target = {pos.x - half_width, pos.y - half_height};
        camera.offset = target.offset;
        camera.zoom = target.zoom;
        camera.rotation = target.rotation;
        BeginMode2D(camera);
        player_render = render;
    }

    const auto view = registry.view<const Position, const RenderComponent>(entt::exclude<Player>);
    view.each([](const Position pos, const RenderComponent renderData)
              { DrawCircleV(Vector2{pos.x, pos.y}, renderData.radius, renderData.color); });

    const auto border_view = registry.view<Border>();
    for (const auto& [ent, border]: border_view.each())
    {
        DrawRectangleRec(border.getRect(), border.color);
    }

    EndMode2D();

    DrawCircleV(Vector2{half_width, half_height}, player_render.radius, player_render.color);

    updateui(registry, delta, status);
}

void DrawSystem::updateui(entt::registry& registry, const float delta, const GameStatus status)
{
    DrawText(TextFormat("Score: %i", status.score), 0.f, 0.f, 28.f, RED);
    DrawText(TextFormat("Enemies left: %i", status.enemies_left), 300.f, 0.f, 28.f, RED);
    DrawText(TextFormat("Next wave: %1.2f", status.next_wave_timer), 580.f, 0.f, 28.f, RED);

    const auto hp_view = registry.view<Health, Player>();
    hp_view.each(
            [](const Health health, Player)
            {
                DrawText(TextFormat("Health: %i / %i", health.points, health.max_points),
                         0.f,
                         Game::m_ScreenHeight - 30.f,
                         28.f,
                         RED);
            });


    AnimationSystem::updateAnimation(registry, delta);
}
