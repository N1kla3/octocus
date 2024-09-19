#pragma once
#include <vector>
#include "entt/entt.hpp"

enum class CollisionChannel
{
    PLAYER,
    BOT
};

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
    CollisionChannel channel;
};

struct collision_resolver
{
    std::vector<entt::entity> hit_entities;
};

struct player
{
    int id;
};

struct bot
{
    int id;
};
