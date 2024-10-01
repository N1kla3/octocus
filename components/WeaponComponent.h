#pragma once

struct WeaponComponent
{
    float damage;
    float range;
    float attack_cooldown_duration;
    float cooldown = 0.f;

    bool attack = false;
};

struct ShootComponent
{
    float damage;
    float cooldown;
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
