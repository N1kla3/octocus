#pragma once
#include <vector>
#include "entt/entt.hpp"

enum class CollisionChannel
{
    PLAYER,
    BOT,
    WEAPON,
    NONE
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
    CollisionChannel responce_channel = CollisionChannel::NONE;
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
