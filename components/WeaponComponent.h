#pragma once

struct WeaponComponent
{
    float damage;
    float range;
    float attack_cooldown_duration;
    bool under_cooldown;
};

struct DeathComponent
{

};

struct DamageHitComponent
{
    float damage;
};
