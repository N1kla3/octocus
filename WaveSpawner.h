#pragma once
#include <array>

class WaveSpawner
{
    inline static std::array<int, 12> s_Waves = { 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240 };
private:
    void spawnRangeEnemy();
    void spawnMeleeEnemy();

    int m_WaveCounter = 0;

public:
    void startWave();
    bool checkWaveFinished();
    void update(float delta);
};
