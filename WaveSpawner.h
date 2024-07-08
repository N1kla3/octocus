#pragma once
#include <array>
#include "entt/entt.hpp"

class WaveSpawner
{
    inline static std::array<int, 12> s_Waves = { 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240 };
private:
    void spawnRangeEnemy(entt::registry& registry);
    void spawnMeleeEnemy(entt::registry& registry);

    size_t m_WaveCounter = 0;
    int m_EnemiesToSpawn = 0;

public:
    void startWave(entt::registry& registry);
    void update(float delta);
};

