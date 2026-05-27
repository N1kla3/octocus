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

    inline bool checkCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
    {
        bool collision = false;

        float dx = center2.x - center1.x; // X distance between centers
        float dy = center2.y - center1.y; // Y distance between centers

        float distance_squared = (dx * dx) + (dy * dy); // Distance between centers squared
        float radius_sum = radius1 + radius2;

        collision = (distance_squared <= (radius_sum * radius_sum));

        return collision;
    }
} // namespace oct
