#pragma once
#include <string>
#include "raylib.h"

struct TextAnimation
{
    std::string text = "empty";
    float max_time = 1.f;
    Vector2 pos{ 0.f, 0.f };
    Color color = RED;
    int font_size = 16;

    float current_time = 0.f;
};
