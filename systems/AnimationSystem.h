#pragma once
#include "entt/entt.hpp"

class AnimationSystem
{
public:
    static void updateAnimation(entt::registry& registry, float deltaTime);
};
