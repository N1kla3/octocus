#include "KillSystem.h"
#include "LifeComponents.h"
#include "WeaponComponent.h"

void KillSystem::update(entt::registry& registry)
{
    auto damage_check_view = registry.view<Damage, Health>();

    damage_check_view.each([&registry](auto entity, Damage& damage, Health& health)
    {
        health.points -= damage.points;
        damage.points = 0;
        if (health.points < 0)
        {
            registry.destroy(entity);
        }
    });

    auto death_view = registry.view<DeathComponent>();

    death_view.each([&registry](auto entity) { registry.destroy(entity); } );
}
