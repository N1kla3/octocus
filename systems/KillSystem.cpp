#include "KillSystem.h"
#include "LifeComponents.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"

void KillSystem::update(entt::registry& registry, float delta, GameStatus& status)
{
    auto cooldown_view = registry.view<Cooldown>();

    cooldown_view.each([delta, &registry](auto entity, Cooldown& cooldown)
    {
        cooldown.duration -= delta;
        if (cooldown.duration < 0.f)
        {
            registry.emplace<DeathComponent>(entity);
        }
    });

    auto damage_check_view = registry.view<Damage, Health>();

    damage_check_view.each([&registry](auto entity, Damage& damage, Health& health)
    {
        health.points -= damage.points;
        damage.points = 0;
        if (health.points < 0)
        {
            registry.emplace<DeathComponent>(entity);
        }
    });
    auto bot_score = registry.view<DeathComponent, bot>();
    bot_score.each([&registry, &status](auto entity) { status.score++; } );

    auto player_dead = registry.view<DeathComponent, player>();
    player_dead.each([&registry, &status](auto entity) { status.is_player_dead = true; } );

    auto death_view = registry.view<DeathComponent>();
    death_view.each([&registry](auto entity) { registry.destroy(entity); } );
}
