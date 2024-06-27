#include "AttackSystem.h"
#include "WeaponComponent.h"
#include "SpaceComponents.h"

void AttackSystem::update(entt::registry& registry)
{
    auto attacker_view = registry.view<WeaponComponent, const position>();

    attacker_view.each([&registry](WeaponComponent weapon, const position pos) 
    {
        // Locate targets 
    });
}
