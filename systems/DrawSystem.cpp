#include "DrawSystem.h"
#include "SpaceComponents.h"
#include "AnimationSystem.h"
#include "RenderComponent.h"
#include "Animation.h"
#include "../game.h"
#include "raylib.h"

void DrawSystem::update(entt::registry& registry, float delta, GameStatus status)
{
    RenderComponent player_render;
    float half_width = static_cast<float>(Game::m_ScreenWidth)/2;
    float half_height = static_cast<float>(Game::m_ScreenHeight)/2;
    auto camera_view = registry.view<const position, const cameraTarget, const RenderComponent>();
    for (const auto& [ent, pos, target, render] : camera_view.each())
    {
        Camera2D camera{};
        camera.target = {pos.x - half_width, pos.y - half_height };
        camera.offset = target.offset;
        camera.zoom = target.zoom;
        camera.rotation = target.rotation;
        BeginMode2D(camera);
        player_render = render;
    }

    auto view = registry.view<const position, const RenderComponent>(entt::exclude<player>);

    view.each([](const position pos, const RenderComponent renderData) 
    {
        DrawCircleV(Vector2{pos.x, pos.y}, renderData.radius, renderData.color);
    });
    EndMode2D();

    DrawCircleV(Vector2{half_width, half_height}, player_render.radius, player_render.color);

    updateui(registry, delta, status);

}

void DrawSystem::updateui(entt::registry& registry, float delta, GameStatus status)
{
    DrawText(TextFormat("Score: %i", status.score), 0.f, 0.f, 28.f, RED);
    DrawText(TextFormat("Enemies left: %i", status.enemies_left), 300.f, 0.f, 28.f, RED);

    if (status.is_player_dead)
    {
        //auto ent = registry.create();
        //registry.emplace<TextAnimation>(ent);
    }

    AnimationSystem::updateAnimation(registry, delta);
}
