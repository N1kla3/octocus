#include "DrawSystem.h"
#include "SpaceComponents.h"
#include "RenderComponent.h"

void DrawSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<const position, const RenderComponent>();

    view.each([](const position pos, const RenderComponent renderData) 
    {
        DrawCircle(pos.x, pos.y, renderData.radius, renderData.color);
    });
}
