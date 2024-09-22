#include "AttackSystem.h"
#include "RenderComponent.h"
#include "WeaponComponent.h"
#include "LifeComponents.h"
#include "SpaceComponents.h"
#include "entt/entity/fwd.hpp"

void AttackSystem::update(entt::registry& registry, float deltaTime)
{
    auto attacker_view = registry.view<WeaponComponent, const position>();

    attacker_view.each([&registry, deltaTime](entt::entity ent, WeaponComponent& weapon, const position pos) 
    {
        weapon.cooldown += deltaTime;
        player* is_player = registry.try_get<player>(ent);
        CollisionChannel channel = is_player ? CollisionChannel::BOT : CollisionChannel::PLAYER;
        if (weapon.attack && weapon.cooldown > weapon.attack_cooldown_duration)
        {
            weapon.cooldown = 0.f;
            auto entity = registry.create();
            registry.emplace<position>(entity, pos);
            registry.emplace<RenderComponent>(entity, RED, 17.f, RenderPriority::LOW);
            registry.emplace<sphere_collision>(entity, 17.f, CollisionChannel::WEAPON, channel);
            registry.emplace<collision_resolver>(entity);
            registry.emplace<DamageHitComponent>(entity, 30.f);
            registry.emplace<Cooldown>(entity, 2.f);
        }
    });

    auto shooter_view = registry.view<ShootComponent, const position>();
    shooter_view.each([&registry, deltaTime](entt::entity ent, ShootComponent& shooter, const position pos)
    {
        shooter.current_cooldown += deltaTime;
        player* is_player = registry.try_get<player>(ent);
        CollisionChannel channel = is_player ? CollisionChannel::BOT : CollisionChannel::PLAYER;
        if (shooter.current_cooldown > shooter.cooldown)
        {
            shooter.current_cooldown = 0.f;
            auto entity = registry.create();
            registry.emplace<position>(entity, pos);
            registry.emplace<velocity>(entity, shooter.target_x*4.f, shooter.target_y*4.f);
            registry.emplace<RenderComponent>(entity, GREEN, 3.f, RenderPriority::LOW);
            registry.emplace<sphere_collision>(entity, 3.f, CollisionChannel::WEAPON, channel);
            registry.emplace<collision_resolver>(entity);
            registry.emplace<DamageHitComponent>(entity, 20.f);
            registry.emplace<Cooldown>(entity, 6.f);
        }
    });
}
