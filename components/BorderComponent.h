#pragma once
#include <algorithm>
#include <cstdlib>
#include "raylib.h"

struct Border
{
    Vector2 left_down_bound;
    Vector2 right_upper_bound;

    Color color;

    [[nodiscard]] Rectangle getRect() const
    {
        Rectangle rect;
        rect.x = std::min(left_down_bound.x, right_upper_bound.x);
        rect.y = std::min(left_down_bound.y, right_upper_bound.y);
        rect.width = std::abs(left_down_bound.x - right_upper_bound.x);
        rect.height = std::abs(left_down_bound.y - right_upper_bound.y);
        return rect;
    }
};

struct BorderCollision
{
    Rectangle rect;
};
