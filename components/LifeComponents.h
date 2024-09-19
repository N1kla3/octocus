#pragma once

struct Health
{
    int points;
    int max_points;
};

struct Damage
{
    float points = 0;
};

struct Cooldown
{
    float duration;
};
