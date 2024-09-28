#pragma once
#include "entt/entt.hpp"
#include "StatusComponents.h"

class DrawSystem
{
public:
    static void update(entt::registry& registry, float delta, GameStatus status);
private:
    static void updateui(entt::registry& reg, float delta, GameStatus status);
};

