#pragma once
#include <algorithm>
#include <cstdlib>
#include "octmath.h"
#include "octrender.h"

struct Border
{
    oct::Vector2 left_down_bound;
    oct::Vector2 right_upper_bound;

    [[nodiscard]] oct::Rectangle getRect() const
    {
        oct::Rectangle rect;
        rect.x = std::min(left_down_bound.x, right_upper_bound.x);
        rect.y = std::min(left_down_bound.y, right_upper_bound.y);
        rect.width = std::abs(left_down_bound.x - right_upper_bound.x);
        rect.height = std::abs(left_down_bound.y - right_upper_bound.y);
        return rect;
    }


#if OCT_RENDER
    [[nodiscard]] Rectangle getRectRender() const
    {
        Rectangle rect;
        rect.x = std::min(left_down_bound.x, right_upper_bound.x);
        rect.y = std::min(left_down_bound.y, right_upper_bound.y);
        rect.width = std::abs(left_down_bound.x - right_upper_bound.x);
        rect.height = std::abs(left_down_bound.y - right_upper_bound.y);
        return rect;
    }
#endif
};

struct BorderCollision
{
    oct::Rectangle rect;
};

#if OCT_RENDER
struct BorderRender
{
    Color color;
};
#endif
