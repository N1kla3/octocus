#pragma once

struct WeaponComponent
{
    float damage = 10.f;
    float range = 10.f;
    float attack_cooldown_duration = 1.f;
    float cooldown = 0.f;

    bool attack = false;
};

struct ShootComponent
{
    float damage = 10.f;
    float cooldown = 1.f;
    float current_cooldown = 0.f;
    float target_x = 0.f;
    float target_y = 0.f;

    bool attack = false;
};

struct DeathComponent
{

};

struct DamageHitComponent
{
    float damage = 0.f;
    bool aoe_hit = false;
    bool hit = false;
};
