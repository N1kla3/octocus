#pragma once
#include "StatusComponents.h"
#include "entt/entt.hpp"

class KillSystem
{
public:
    static void update(entt::registry& registry, float delta, GameStatus& status);
};
