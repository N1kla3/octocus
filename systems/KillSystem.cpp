#include "KillSystem.h"
#include "Animation.h"
#include "LifeComponents.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"

void KillSystem::update(entt::registry& registry, float delta, GameStatus& status)
{
    const auto cooldown_view = registry.view<Cooldown>();

    cooldown_view.each(
            [delta, &registry](auto entity, Cooldown& cooldown)
            {
                cooldown.duration -= delta;
                if (cooldown.duration < 0.f)
                {
                    registry.emplace<DeathComponent>(entity);
                }
            });

    const auto projectile_view = registry.view<DamageHitComponent>();
    for (const auto& [entity, projectile]: projectile_view.each())
    {
        if (projectile.hit)
        {
            registry.destroy(entity);
        }
    }

    const auto damage_check_view = registry.view<Damage, Health>();

    damage_check_view.each(
            [&registry](auto entity, Damage& damage, Health& health)
            {
                health.points -= static_cast<int>(damage.points);
                damage.points = 0;
                if (health.points <= 0)
                {
                    registry.emplace<DeathComponent>(entity);
                }
            });
    const auto bot_score = registry.view<DeathComponent, Bot>();
    bot_score.each(
            [&status]([[maybe_unused]] auto entity)
            {
                status.score++;
                status.enemies_left--;
            });

    const auto player_dead = registry.view<DeathComponent, Player>();
    player_dead.each(
            [&registry, &status]([[maybe_unused]] auto entity)
            {
                status.is_player_dead = true;
                const auto ent = registry.create();
                registry.emplace<TextAnimation>(ent);
            });

    const auto death_view = registry.view<DeathComponent>();
    death_view.each([&registry](auto entity) { registry.destroy(entity); });
}
