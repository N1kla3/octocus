#include "InputSystem.h"
#include <cmath>
#include "../game.h"
#include "WeaponComponent.h"
#include "SpaceComponents.h"
#include "raylib.h"
#include "raymath.h"

void InputSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<velocity, player>();

    bool request_attack = false;
    velocity vel;
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
    float len = std::sqrt(vel.dx * vel.dx + vel.dy * vel.dy);
    if (len != 0.f)
    {
        vel.dx /= len;
        vel.dy /= len;
    }
    vel.dx *= 140.f;
    vel.dy *= 140.f;

    if (IsKeyDown(KEY_SPACE))
    {
        request_attack = true;
    }

    view.each([vel](velocity& veloc, player pl) 
    {
        veloc = vel;
    });

    auto weapon_view = registry.view<WeaponComponent, ShootComponent, position, player>(); 


    float mousex = static_cast<float>(GetMouseX());
    float mousey = static_cast<float>(GetMouseY());


    weapon_view.each([mousex, mousey, request_attack](WeaponComponent& weap, ShootComponent& shoot, position pos, player pl)
    {
        Vector2 res = Vector2Normalize({mousex - static_cast<float>(Game::m_ScreenWidth)/2, mousey - static_cast<float>(Game::m_ScreenHeight)/2 });
        shoot.target_x = res.x;
        shoot.target_y = res.y;
        weap.attack = request_attack;
    });
}
