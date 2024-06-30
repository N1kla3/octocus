#include "DamageSystem.h"
#include "WeaponComponent.h"
#include "SpaceComponents.h"
#include "LifeComponents.h"

void DamageSystem::updateDamage(entt::registry &registry)
{
    auto view = registry.view<collision_resolver, Damage>();

    view.each([&registry](auto entity, const collision_resolver& resolver, Damage& damage)
    {
        for (entt::entity hit : resolver.hit_entities)
        {
            DamageHitComponent* hit_damage = registry.try_get<DamageHitComponent>(hit);
            if (hit_damage)
            {
                damage.points += hit_damage->damage;
                registry.emplace<DeathComponent>(hit);
            }
        }
    });
}
