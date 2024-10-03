#pragma once
#include "raylib.h"
#include <cstdint>

enum class RenderPriority : uint8_t
{
    LOW,
    MIDLE,
    HIGH
};

struct RenderComponent
{
    Color color = RED;
    float radius = 10.f;
    RenderPriority priority = RenderPriority::LOW;
};
