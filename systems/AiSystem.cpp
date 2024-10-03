#include "AiSystem.h"
#include "AiComponents.h"
#include "../game.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"
#include "raylib.h"
#include "raymath.h"

void AiSystem::updateAi(entt::registry &registry, float delta)
{
    auto player_v = registry.view<position, player>();
    Vector2 player_pos;
    player_v.each([&player_pos](position pos, player){ player_pos.x = pos.x; player_pos.y = pos.y; });

    auto range_v = registry.view<RangeAi, bot, position, ShootComponent, velocity>();
    range_v.each([delta, player_pos](RangeAi& range, bot, position& pos, ShootComponent& shoot, velocity& vel)
    {
        Vector2 my_pos = pos.toVector2();
        float distance = Vector2Distance(my_pos, player_pos);

        if (distance < range.shoot_distance)
        {
            if (distance < range.safe_distance)
            {
                Vector2 direction = Vector2Normalize(Vector2Subtract(my_pos, player_pos));
                vel = velocity(Vector2Scale(direction, range.speed));
            }
            else
            {
                if (range.curr_strafe_duration > range.strafe_duration)
                {
                    int value = GetRandomValue(1, 2);
                    range.strafe_angle = value == 1 ? 90 : -90;
                    range.curr_strafe_duration = 0;
                }
                else
                {
                    Vector2 direction = Vector2Normalize(Vector2Subtract(player_pos, my_pos));
                    direction = Vector2Rotate(direction, range.strafe_angle);
                    vel = velocity(Vector2Scale(direction, range.speed));
                    range.curr_strafe_duration += delta;
                }
            }
            shoot.target_x = player_pos.x - my_pos.x;
            shoot.target_y = player_pos.y - my_pos.y;
            shoot.attack = true;

        }
        else
        {
            Vector2 direction = Vector2Normalize(Vector2Subtract(player_pos, my_pos));
            vel = velocity(Vector2Scale(direction, range.speed));
            shoot.attack = false;
        }
    });

    auto melee_v = registry.view<MeleeAi, bot, position, WeaponComponent, velocity>();
    melee_v.each([player_pos](MeleeAi& mel, bot, position& pos, WeaponComponent& weap, velocity& vel)
    {

    });
}
