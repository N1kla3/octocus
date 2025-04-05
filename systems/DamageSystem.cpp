#include "DamageSystem.h"
#include "LifeComponents.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"

void DamageSystem::updateDamage(entt::registry& registry)
{
    const auto view = registry.view<CollisionResolver, DamageHitComponent>();

    view.each(
            [&registry](auto /*entity*/, const CollisionResolver& resolver, DamageHitComponent& damage)
            {
                for (entt::entity const hit: resolver.hit_entities)
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
