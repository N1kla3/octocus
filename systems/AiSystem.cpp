#include "AiSystem.h"
#include "AiComponents.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"
#if OCT_CLIENT
#include "octrender.h"
#endif


void AiSystem::updateAi(entt::registry& registry, float delta)
{
    const auto player_v = registry.view<Position, Player>();
    oct::Vector2 player_pos;
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
                oct::Vector2 const my_pos = pos.toVector2();
                float const distance = my_pos.distance(player_pos);

                shoot.attack = false;
                if (distance < range.shoot_distance)
                {
                    if (distance < range.safe_distance)
                    {
                        oct::Vector2 direction = (my_pos - player_pos);
                        direction.normalizeInline();
                        direction.scale(range.speed);
                        vel = Velocity(direction);
                    }
                    else
                    {
                        if (range.curr_strafe_duration > range.strafe_duration)
                        {
                            // TODO
                            // int const value = GetRandomValue(1, 2);
                            const int value = 1;
                            range.strafe_angle = value == 1 ? 90 : -90;
                            range.curr_strafe_duration = 0;
                        }
                        else
                        {
                            oct::Vector2 direction = (player_pos - my_pos);
                            direction.normalizeInline();
                            direction.rotate(range.strafe_angle);
                            direction.scale(range.speed);
                            vel = Velocity(direction);
                            range.curr_strafe_duration += delta;
                        }
                    }
                    shoot.target_x = player_pos.x - my_pos.x;
                    shoot.target_y = player_pos.y - my_pos.y;
                    shoot.attack = true;
                }

                else
                {
                    oct::Vector2 direction = (player_pos - my_pos);
                    direction.normalizeInline();
                    direction.scale(range.speed);
                    vel = Velocity(direction);
                }
            });

    auto melee_v = registry.view<MeleeAi, Bot, Position, WeaponComponent, Velocity>();
    melee_v.each(
            [player_pos](const MeleeAi& mel, Bot, const Position& pos, WeaponComponent& weap, Velocity& vel)
            {
                oct::Vector2 const my_pos = pos.toVector2();
                float const distance = my_pos.distance(player_pos);

                oct::Vector2 direction = (player_pos - my_pos).normalize();

                weap.attack = distance < mel.attack_distance;

                if (distance < mel.haste_distance)
                {
                    direction.scale(mel.speed + 20.f);
                }
                else
                {
                    direction.scale(mel.speed);
                }
                vel = Velocity(direction);
            });
}
