#pragma once
#include "entt/entt.hpp"

class DrawSystem
{
public:
    static void update(entt::registry& registry, float delta);
private:
    static void updateui(entt::registry& reg, float delta);
};

