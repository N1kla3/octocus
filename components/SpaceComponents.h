#pragma once
#include <vector>
#include "entt/entt.hpp"

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

struct sphere_collision
{
    float radius;
};

struct collision_resolver
{
    std::vector<entt::entity> hit_entities;
};

struct player
{

};
