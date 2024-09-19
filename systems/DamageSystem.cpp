#include "DamageSystem.h"
#include "WeaponComponent.h"
#include "SpaceComponents.h"
#include <iostream>
#include "LifeComponents.h"

void DamageSystem::updateDamage(entt::registry &registry)
{
    auto view = registry.view<collision_resolver, DamageHitComponent>();

    view.each([&registry](auto entity, const collision_resolver& resolver, DamageHitComponent& damage)
    {
        for (entt::entity hit : resolver.hit_entities)
        {
            Damage* hit_damage = registry.try_get<Damage>(hit);
            if (hit_damage)
            {
                hit_damage->points += damage.damage;
                registry.emplace<DeathComponent>(hit);
                std::cout << "Hit" << std::endl;
            }
        }
    });
}
