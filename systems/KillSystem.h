#pragma once
#include "entt/entt.hpp"
#include "StatusComponents.h"

class KillSystem
{
public:
    static void update(entt::registry& registry, float delta, GameStatus& status);
};
