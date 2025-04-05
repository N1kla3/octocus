#pragma once
#include <algorithm>
#include <cstdlib>
#include "raylib.h"
#include "raymath.h"

struct border
{
    Vector2 LeftDownBound;
    Vector2 RightUpperBound;

    Color color;

    [[nodiscard]] Rectangle getRect() const
    {
        Rectangle r;
        r.x = std::min(LeftDownBound.x, RightUpperBound.x);
        r.y = std::min(LeftDownBound.y, RightUpperBound.y);
        r.width = std::abs(LeftDownBound.x - RightUpperBound.x);
        r.height = std::abs(LeftDownBound.y - RightUpperBound.y);
        return r;
    }
};

struct border_collision
{
    Rectangle rect;
};
