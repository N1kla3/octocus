#pragma once

struct WeaponComponent
{
    float damage;
    float range;
    float attack_cooldown_duration;
    bool under_cooldown;

    bool attack = false;
};

struct ShootComponent
{
    float damage;
    float cooldown;
    float current_cooldown;
    float target_x;
    float target_y;

    bool attack = false;
};

struct DeathComponent
{

};

struct DamageHitComponent
{
    float damage;
};
