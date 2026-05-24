#pragma once
#include <vector>
#include "../math/octmath.h"
#include "entt/entt.hpp"


enum class CollisionChannel : uint8_t
{
    PLAYER,
    BOT,
    WEAPON,
    NONE
};

struct Position
{
    float x;
    float y;
    explicit Position(float posx = 0.f, float posy = 0.f)
        : x(posx)
        , y(posy)
    {
    }
    explicit Position(const oct::Vector2 vec)
    {
        x = vec.x;
        y = vec.y;
    }
    [[nodiscard]] oct::Vector2 toVector2() const
    {
        return oct::Vector2{x, y};
    }
};

struct Velocity
{
    float dx;
    float dy;

    explicit Velocity(float velx = 0.f, float vely = 0.f)
        : dx(velx)
        , dy(vely)
    {
    }
    explicit Velocity(const oct::Vector2 vec)
    {
        dx = vec.x;
        dy = vec.y;
    }
    [[nodiscard]] oct::Vector2 toVector2() const
    {
        return oct::Vector2{dx, dy};
    }
};

struct SphereCollision
{
    float radius = 10.f;
    CollisionChannel channel = CollisionChannel::NONE;
    CollisionChannel responce_channel = CollisionChannel::NONE;
};

struct CollisionResolver
{
    std::vector<entt::entity> hit_entities;
};

struct Player
{
    int id;
};

struct Bot
{
    int id;
};

struct CameraTarget
{
    oct::Vector2 offset = {0.f, 0.f};
    float rotation = 0.f;
    float zoom = 1.f;
};
