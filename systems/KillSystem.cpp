#include "KillSystem.h"
#include "Animation.h"
#include "LifeComponents.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"

void KillSystem::update(entt::registry& registry, float delta, GameStatus& status)
{
    auto cooldown_view = registry.view<Cooldown>();

    cooldown_view.each(
            [delta, &registry](auto entity, Cooldown& cooldown)
            {
                cooldown.duration -= delta;
                if (cooldown.duration < 0.f)
                {
                    registry.emplace<DeathComponent>(entity);
                }
            });

    auto projectile_view = registry.view<DamageHitComponent>();
    for (const auto& [entity, projectile]: projectile_view.each())
    {
        if (projectile.hit)
        {
            registry.destroy(entity);
        }
    }

    auto damage_check_view = registry.view<Damage, Health>();

    damage_check_view.each(
            [&registry](auto entity, Damage& damage, Health& health)
            {
                health.points -= damage.points;
                damage.points = 0;
                if (health.points <= 0)
                {
                    registry.emplace<DeathComponent>(entity);
                }
            });
    auto bot_score = registry.view<DeathComponent, bot>();
    bot_score.each(
            [&registry, &status](auto entity)
            {
                status.score++;
                status.enemies_left--;
            });

    auto player_dead = registry.view<DeathComponent, player>();
    player_dead.each(
            [&registry, &status](auto entity)
            {
                status.is_player_dead = true;
                auto ent = registry.create();
                registry.emplace<TextAnimation>(ent);
            });

    auto death_view = registry.view<DeathComponent>();
    death_view.each([&registry](auto entity) { registry.destroy(entity); });
}
