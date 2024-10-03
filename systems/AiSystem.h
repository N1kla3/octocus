#pragma once 
#include "entt/entt.hpp"

class AiSystem
{
public:
    static void updateAi(entt::registry& registry, float delta);
};
