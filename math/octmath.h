#pragma once

#include <cmath>

namespace oct
{
    struct Vector2
    {
        float x, y;

        float distance(Vector2 rhs) const
        {
            float result = sqrtf(((x - rhs.x) * (x - rhs.x)) + ((y - rhs.y) * (y - rhs.y)));

            return result;
        }
        Vector2 normalize() const
        {
            Vector2 result = {0};
            float length = sqrtf((x * x) + (y * y));

            if (length > 0)
            {
                float ilength = 1.0f / length;
                result.x = x * ilength;
                result.y = y * ilength;
            }

            return result;
        }
        void normalizeInline()
        {
            Vector2 result = {0};
            float length = sqrtf((x * x) + (y * y));

            if (length > 0)
            {
                float ilength = 1.0f / length;
                x = x * ilength;
                y = y * ilength;
            }
        }

        void scale(float scale)
        {
            x = x * scale;
            y = y * scale;
        }

        void rotate(float angle)
        {
            float cosres = cosf(angle);
            float sinres = sinf(angle);

            x = (x * cosres) - (y * sinres);
            y = (x * sinres) + (y * cosres);
        }

        friend Vector2 operator-(Vector2 lhs, Vector2 rhs)
        {
            return lhs.subtract(rhs);
        }
        friend Vector2 operator+(Vector2 lhs, Vector2 rhs)
        {
            Vector2 result = {lhs.x + rhs.x, lhs.y + rhs.y};

            return result;
        }

        Vector2 subtract(Vector2 other) const
        {
            Vector2 result = {x - other.x, y - other.y};

            return result;
        }
    };

    // Rectangle, 4 components
    struct Rectangle
    {
        float x; // Rectangle top-left corner position x
        float y; // Rectangle top-left corner position y
        float width; // Rectangle width
        float height; // Rectangle height
    };

} // namespace oct
