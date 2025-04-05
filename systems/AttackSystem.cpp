#include "AttackSystem.h"
#include "LifeComponents.h"
#include "RenderComponent.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"
#include "entt/entity/fwd.hpp"
#include "raymath.h"

void AttackSystem::update(entt::registry& registry, float deltaTime)
{
    const auto attacker_view = registry.view<WeaponComponent, const Position>();

    attacker_view.each(
            [&registry, deltaTime](const entt::entity ent, WeaponComponent& weapon, const Position pos)
            {
                weapon.cooldown += deltaTime;
                const Player* is_player = registry.try_get<Player>(ent);
                CollisionChannel const channel = is_player ? CollisionChannel::BOT : CollisionChannel::PLAYER;
                if (weapon.attack && weapon.cooldown > weapon.attack_cooldown_duration)
                {
                    weapon.cooldown = 0.f;
                    const auto entity = registry.create();
                    registry.emplace<Position>(entity, pos);
                    registry.emplace<RenderComponent>(entity, RED, weapon.range, RenderPriority::LOW);
                    registry.emplace<SphereCollision>(entity, weapon.range, CollisionChannel::WEAPON, channel);
                    registry.emplace<CollisionResolver>(entity);
                    registry.emplace<DamageHitComponent>(entity, weapon.damage, true);
                    registry.emplace<Cooldown>(entity, 2.f);
                }
            });

    const auto shooter_view = registry.view<ShootComponent, const Position>();
    shooter_view.each(
            [&registry, deltaTime](const entt::entity ent, ShootComponent& shooter, const Position pos)
            {
                shooter.current_cooldown += deltaTime;
                const Player* is_player = registry.try_get<Player>(ent);
                CollisionChannel const channel = is_player ? CollisionChannel::BOT : CollisionChannel::PLAYER;
                if (shooter.attack && shooter.current_cooldown > shooter.cooldown)
                {
                    shooter.current_cooldown = 0.f;
                    Vector2 direction = {shooter.target_x, shooter.target_y};
                    direction = Vector2Normalize(direction);
                    const auto entity = registry.create();
                    registry.emplace<Position>(entity, pos);
                    registry.emplace<Velocity>(entity, Vector2Scale(direction, 180.f));
                    registry.emplace<RenderComponent>(entity, GREEN, 3.f, RenderPriority::LOW);
                    registry.emplace<SphereCollision>(entity, 3.f, CollisionChannel::WEAPON, channel);
                    registry.emplace<CollisionResolver>(entity);
                    registry.emplace<DamageHitComponent>(entity, shooter.damage);
                    registry.emplace<Cooldown>(entity, 6.f);
                }
            });
}
