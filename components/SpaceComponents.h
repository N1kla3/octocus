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
    explicit position(float x = 0.f, float y = 0.f)
        : x(x), y(y){}
    explicit position(const Vector2 vec)
    {
        x = vec.x;
        y = vec.y;
    }
    [[nodiscard]] Vector2 toVector2() const { return Vector2{x, y}; }
};

struct velocity {
    float dx;
    float dy;

    explicit velocity(float x = 0.f, float y = 0.f)
        : dx(x), dy(y){}
    explicit velocity(const Vector2 vec)
    {
        dx = vec.x;
        dy = vec.y;
    }
    [[nodiscard]] Vector2 toVector2() const { return Vector2{dx, dy}; }
};

struct sphere_collision
{
    float radius = 10.f;
    CollisionChannel channel = CollisionChannel::NONE;
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
