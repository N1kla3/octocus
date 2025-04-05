#include "AiSystem.h"
#include "../game.h"
#include "AiComponents.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"
#include "raylib.h"
#include "raymath.h"

void AiSystem::updateAi(entt::registry& registry, float delta)
{
    const auto player_v = registry.view<Position, Player>();
    Vector2 player_pos;
    player_v.each(
            [&player_pos](Position pos, Player)
            {
                player_pos.x = pos.x;
                player_pos.y = pos.y;
            });

    auto range_v = registry.view<RangeAi, Bot, Position, ShootComponent, Velocity>();
    range_v.each(
            [delta, player_pos](RangeAi& range, Bot, const Position& pos, ShootComponent& shoot, Velocity& vel)
            {
                Vector2 const my_pos = pos.toVector2();
                float const distance = Vector2Distance(my_pos, player_pos);

                shoot.attack = false;
                if (distance < range.shoot_distance)
                {
                    if (distance < range.safe_distance)
                    {
                        Vector2 const direction = Vector2Normalize(Vector2Subtract(my_pos, player_pos));
                        vel = Velocity(Vector2Scale(direction, range.speed));
                    }
                    else
                    {
                        if (range.curr_strafe_duration > range.strafe_duration)
                        {
                            int const value = GetRandomValue(1, 2);
                            range.strafe_angle = value == 1 ? 90 : -90;
                            range.curr_strafe_duration = 0;
                        }
                        else
                        {
                            Vector2 direction = Vector2Normalize(Vector2Subtract(player_pos, my_pos));
                            direction = Vector2Rotate(direction, range.strafe_angle);
                            vel = Velocity(Vector2Scale(direction, range.speed));
                            range.curr_strafe_duration += delta;
                        }
                    }
                    shoot.target_x = player_pos.x - my_pos.x;
                    shoot.target_y = player_pos.y - my_pos.y;
                    shoot.attack = true;
                }

                else
                {
                    Vector2 const direction = Vector2Normalize(Vector2Subtract(player_pos, my_pos));
                    vel = Velocity(Vector2Scale(direction, range.speed));
                }
            });

    auto melee_v = registry.view<MeleeAi, Bot, Position, WeaponComponent, Velocity>();
    melee_v.each(
            [player_pos](const MeleeAi& mel, Bot, const Position& pos, WeaponComponent& weap, Velocity& vel)
            {
                Vector2 const my_pos = pos.toVector2();
                float const distance = Vector2Distance(my_pos, player_pos);

                Vector2 const direction = Vector2Normalize(Vector2Subtract(player_pos, my_pos));

                weap.attack = distance < mel.attack_distance;

                if (distance < mel.haste_distance)
                {
                    vel = Velocity(Vector2Scale(direction, mel.speed + 20.f));
                }
                else
                {
                    vel = Velocity(Vector2Scale(direction, mel.speed));
                }
            });
}
