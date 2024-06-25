#pragma once
#include "entt/entt.hpp"

class InputSystem
{
public:
    static void update(entt::registry& registry, float delta);
};
