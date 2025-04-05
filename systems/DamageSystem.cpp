#include "DamageSystem.h"
#include "LifeComponents.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"

void DamageSystem::updateDamage(entt::registry& registry)
{
    auto view = registry.view<collision_resolver, DamageHitComponent>();

    view.each(
            [&registry](auto entity, const collision_resolver& resolver, DamageHitComponent& damage)
            {
                for (entt::entity hit: resolver.hit_entities)
                {
                    Damage* hit_damage = registry.try_get<Damage>(hit);
                    if (hit_damage)
                    {
                        hit_damage->points += damage.damage;
                        damage.hit = true;
                        if (!damage.aoe_hit)
                        {
                            return;
                        }
                    }
                }
            });
}
