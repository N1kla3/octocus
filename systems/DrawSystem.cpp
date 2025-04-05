#include "DrawSystem.h"
#include "../game.h"
#include "Animation.h"
#include "AnimationSystem.h"
#include "BorderComponent.h"
#include "LifeComponents.h"
#include "RenderComponent.h"
#include "SpaceComponents.h"
#include "raylib.h"

void DrawSystem::update(entt::registry& registry, float delta, GameStatus status)
{
    RenderComponent player_render{};
    float half_width = static_cast<float>(Game::m_ScreenWidth) / 2;
    float half_height = static_cast<float>(Game::m_ScreenHeight) / 2;
    auto camera_view = registry.view<const position, const cameraTarget, const RenderComponent>();
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

    auto view = registry.view<const position, const RenderComponent>(entt::exclude<player>);
    view.each([](const position pos, const RenderComponent renderData)
              { DrawCircleV(Vector2{pos.x, pos.y}, renderData.radius, renderData.color); });

    auto border_view = registry.view<border>();
    for (const auto& [ent, border]: border_view.each())
    {
        DrawRectangleRec(border.getRect(), border.color);
    }

    EndMode2D();

    DrawCircleV(Vector2{half_width, half_height}, player_render.radius, player_render.color);

    updateui(registry, delta, status);
}

void DrawSystem::updateui(entt::registry& registry, float delta, GameStatus status)
{
    DrawText(TextFormat("Score: %i", status.score), 0.f, 0.f, 28.f, RED);
    DrawText(TextFormat("Enemies left: %i", status.enemies_left), 300.f, 0.f, 28.f, RED);
    DrawText(TextFormat("Next wave: %1.2f", status.next_wave_timer), 580.f, 0.f, 28.f, RED);

    auto hp_view = registry.view<Health, player>();
    hp_view.each(
            [](Health hp, player)
            {
                DrawText(TextFormat("Health: %i / %i", hp.points, hp.max_points),
                         0.f,
                         Game::m_ScreenHeight - 30.f,
                         28.f,
                         RED);
            });


    AnimationSystem::updateAnimation(registry, delta);
}
