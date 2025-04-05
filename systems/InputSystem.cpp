#include "InputSystem.h"
#include <cmath>
#include "../game.h"
#include "SpaceComponents.h"
#include "WeaponComponent.h"
#include "raylib.h"
#include "raymath.h"

void InputSystem::update(entt::registry& registry, float /*delta*/)
{
    const auto view = registry.view<Velocity, Player>();

    bool request_attack = false;
    Velocity vel;
    vel.dx = 0.f;
    vel.dy = 0.f;
    if (IsKeyDown(KEY_W))
    {
        vel.dy += -1.f;
    }
    if (IsKeyDown(KEY_S))
    {
        vel.dy += 1.f;
    }
    if (IsKeyDown(KEY_D))
    {
        vel.dx += 1.f;
    }
    if (IsKeyDown(KEY_A))
    {
        vel.dx += -1.f;
    }
    const float len = std::sqrt((vel.dx * vel.dx) + (vel.dy * vel.dy));
    if (len != 0.f)
    {
        vel.dx /= len;
        vel.dy /= len;
    }
    vel.dx *= 160.f;
    vel.dy *= 160.f;

    if (IsKeyDown(KEY_SPACE))
    {
        request_attack = true;
    }

    view.each([vel](Velocity& veloc, Player /*pl*/) { veloc = vel; });

    const auto weapon_view = registry.view<WeaponComponent, ShootComponent, Position, Player>();


    float const mousex = static_cast<float>(GetMouseX());
    float const mousey = static_cast<float>(GetMouseY());


    weapon_view.each(
            [mousex, mousey, request_attack](
                    WeaponComponent& weap, ShootComponent& shoot, Position /*pos*/, Player /*pl*/)
            {
                Vector2 const res = Vector2Normalize({mousex - (static_cast<float>(Game::m_ScreenWidth) / 2),
                                                      mousey - (static_cast<float>(Game::m_ScreenHeight) / 2)});
                shoot.target_x = res.x;
                shoot.target_y = res.y;
                shoot.attack = true;
                weap.attack = request_attack;
            });
}
