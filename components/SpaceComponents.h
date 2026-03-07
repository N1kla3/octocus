#pragma once
#include <vector>
#include "entt/entt.hpp"
#if OCT_CLIENT
#include "raylib.h"
#include "raymath.h"
#endif


#if OCT_SERVER
struct Vector2
{
    float x, y;
};
// Rectangle, 4 components
typedef struct Rectangle
{
    float x; // Rectangle top-left corner position x
    float y; // Rectangle top-left corner position y
    float width; // Rectangle width
    float height; // Rectangle height
} Rectangle;
// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color
{
    unsigned char r; // Color red value
    unsigned char g; // Color green value
    unsigned char b; // Color blue value
    unsigned char a; // Color alpha value
} Color;
inline float Vector2Distance(Vector2 v1, Vector2 v2)
{
    float result = sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));

    return result;
}
inline Vector2 Vector2Normalize(Vector2 v)
{
    Vector2 result = {0};
    float length = sqrtf((v.x * v.x) + (v.y * v.y));

    if (length > 0)
    {
        float ilength = 1.0f / length;
        result.x = v.x * ilength;
        result.y = v.y * ilength;
    }

    return result;
}
inline Vector2 Vector2Scale(Vector2 v, float scale)
{
    Vector2 result = {v.x * scale, v.y * scale};

    return result;
}
inline Vector2 Vector2Rotate(Vector2 v, float angle)
{
    Vector2 result = {0};

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x * cosres - v.y * sinres;
    result.y = v.x * sinres + v.y * cosres;

    return result;
}
inline Vector2 Vector2Subtract(Vector2 v1, Vector2 v2)
{
    Vector2 result = {v1.x - v2.x, v1.y - v2.y};

    return result;
}
#endif

#if OCT_SERVER
inline bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec)
{
    bool collision = false;

    float recCenterX = rec.x + rec.width / 2.0f;
    float recCenterY = rec.y + rec.height / 2.0f;

    float dx = fabsf(center.x - recCenterX);
    float dy = fabsf(center.y - recCenterY);

    if (dx > (rec.width / 2.0f + radius))
    {
        return false;
    }
    if (dy > (rec.height / 2.0f + radius))
    {
        return false;
    }

    if (dx <= (rec.width / 2.0f))
    {
        return true;
    }
    if (dy <= (rec.height / 2.0f))
    {
        return true;
    }

    float cornerDistanceSq =
            (dx - rec.width / 2.0f) * (dx - rec.width / 2.0f) + (dy - rec.height / 2.0f) * (dy - rec.height / 2.0f);

    collision = (cornerDistanceSq <= (radius * radius));

    return collision;
}
#endif

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
    explicit Position(const Vector2 vec)
    {
        x = vec.x;
        y = vec.y;
    }
    [[nodiscard]] Vector2 toVector2() const
    {
        return Vector2{x, y};
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
    explicit Velocity(const Vector2 vec)
    {
        dx = vec.x;
        dy = vec.y;
    }
    [[nodiscard]] Vector2 toVector2() const
    {
        return Vector2{dx, dy};
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
    Vector2 offset = {0.f, 0.f};
    float rotation = 0.f;
    float zoom = 1.f;
};
