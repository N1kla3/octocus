#pragma once
#include "StatusComponents.h"
#include "entt/entt.hpp"

class DrawSystem
{
public:
    static void update(entt::registry& registry, float delta, GameStatus status);

private:
    static void updateui(entt::registry& reg, float delta, GameStatus status);
};
