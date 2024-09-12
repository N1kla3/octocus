#include "AttackSystem.h"
#include "RenderComponent.h"
#include "WeaponComponent.h"
#include "SpaceComponents.h"

void AttackSystem::update(entt::registry& registry, float deltaTime)
{
    auto attacker_view = registry.view<WeaponComponent, const position>();

    attacker_view.each([&registry](WeaponComponent weapon, const position pos) 
    {
        // Locate targets 
    });

    auto shooter_view = registry.view<ShootComponent, const position>();
    shooter_view.each([&registry, deltaTime](ShootComponent& shooter, const position pos)
    {
        shooter.current_cooldown += deltaTime;
        if (shooter.current_cooldown > shooter.cooldown)
        {
            shooter.current_cooldown = 0.f;
            auto entity = registry.create();
            registry.emplace<position>(entity, pos);
            registry.emplace<velocity>(entity, shooter.target_x, shooter.target_y);
            registry.emplace<RenderComponent>(entity, GREEN, 3.f, RenderPriority::LOW);
        }
    });
}
