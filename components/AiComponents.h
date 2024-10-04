#pragma once
#include "raylib.h"

struct RangeAi
{
    Vector2 target{0.f, 0.f};
    float speed = 35.f;
    float safe_distance = 99.f;
    float shoot_distance = 200.f;
    float strafe_duration = 1.2f;
    float strafe_angle = 90.f;

    float curr_strafe_duration = 0.f;
};

struct MeleeAi
{
    Vector2 target{0.f, 0.f};
    float speed = 45.f;
    float attack_distance = 30.f;
    float haste_distance = 110.f;
};

