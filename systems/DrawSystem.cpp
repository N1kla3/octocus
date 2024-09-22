#include "DrawSystem.h"
#include "SpaceComponents.h"
#include "RenderComponent.h"
#include "../game.h"
#include "raylib.h"

void DrawSystem::update(entt::registry& registry, float delta)
{
    auto camera_view = registry.view<const position, const cameraTarget>();
    camera_view.each([](const position pos, const cameraTarget target)
    {
        Camera2D camera{};
        camera.target = {pos.x - static_cast<float>(Game::m_ScreenWidth)/2, pos.y - static_cast<float>(Game::m_ScreenHeight)/2 };
        camera.offset = target.offset;
        camera.zoom = target.zoom;
        camera.rotation = target.rotation;
        BeginMode2D(camera);
    });

    auto view = registry.view<const position, const RenderComponent>();

    view.each([](const position pos, const RenderComponent renderData) 
    {
        DrawCircle(pos.x, pos.y, renderData.radius, renderData.color);
    });
    EndMode2D();

    DrawText("Hello", 0.f, 0.f, 10.f, RED);
}
