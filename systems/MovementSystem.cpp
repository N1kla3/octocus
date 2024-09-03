#include "MovementSystem.h"
#include "SpaceComponents.h"

void MovementSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<const velocity, position, player>();

    view.each([](const velocity vel, position& pos) 
    {
        pos.x += vel.dx;
        pos.y += vel.dy;
    });
}
