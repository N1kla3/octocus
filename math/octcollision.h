#pragma once
#include "octmath.h"

namespace oct
{
    inline bool checkCollisionCircleRec(Vector2 center, float radius, Rectangle rec)
    {
        bool collision = false;

        float rec_center_x = rec.x + (rec.width / 2.0f);
        float rec_center_y = rec.y + (rec.height / 2.0f);

        float dx = fabsf(center.x - rec_center_x);
        float dy = fabsf(center.y - rec_center_y);

        if (dx > ((rec.width / 2.0f) + radius))
        {
            return false;
        }
        if (dy > ((rec.height / 2.0f) + radius))
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

        float corner_distance_sq = ((dx - (rec.width / 2.0f)) * (dx - (rec.width / 2.0f))) +
                                   ((dy - (rec.height / 2.0f)) * (dy - (rec.height / 2.0f)));

        collision = (corner_distance_sq <= (radius * radius));

        return collision;
    }
} // namespace oct
