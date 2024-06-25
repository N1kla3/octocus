#include "MovementSystem.h"
#include "SpaceComponents.h"

void MovementSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<const velocity, position>();

    view.each([](const velocity vel, position& pos) 
    {
        pos.x += vel.dx;
    });
}
