#pragma once
#include "entt/entt.hpp"

class MovementSystem
{
public:
    static void update(entt::registry& registry, float delta);
};
