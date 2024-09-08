#include "InputSystem.h"
#include <cmath>
#include "WeaponComponent.h"
#include "SpaceComponents.h"
#include "raylib.h"

void InputSystem::update(entt::registry& registry, float delta)
{
    auto view = registry.view<velocity, player>();

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

    view.each([vel](velocity& veloc) 
    {
        veloc = vel;
    });

    auto weapon_view = registry.view<ShootComponent, player>(); 

    float mousex = static_cast<float>(GetMouseX());
    float mousey = static_cast<float>(GetMouseY());
    weapon_view.each([mousex, mousey](ShootComponent& shoot)
    {
        shoot.target_x = mousex;
        shoot.target_y = mousey;
    });
}
