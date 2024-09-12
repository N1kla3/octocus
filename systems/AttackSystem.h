#pragma once 
#include "entt/entt.hpp"

class AttackSystem final
{
public:
    static void update(entt::registry& registry, float deltaTime);
};
