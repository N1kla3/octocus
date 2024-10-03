#pragma once
#include <vector>
#include "raylib.h"
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
    position(float x = 0.f, float y = 0.f)
        : x(x), y(y){}
    position(const Vector2 vec)
    {
        x = vec.x;
        y = vec.y;
    }
    Vector2 toVector2() const { return Vector2{x, y}; }
};

struct velocity {
    float dx;
    float dy;

    velocity(float x = 0.f, float y = 0.f)
        : dx(x), dy(y){}
    velocity(const Vector2 vec)
    {
        dx = vec.x;
        dy = vec.y;
    }
    Vector2 toVector2() const { return Vector2{dx, dy}; }
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

struct cameraTarget
{
    Vector2 offset = {0.f, 0.f};
    float rotation = 0.f;
    float zoom = 1.f;

};
