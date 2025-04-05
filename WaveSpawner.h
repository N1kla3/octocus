#pragma once
#include <array>
#include <random>

class WaveSpawner
{
    struct SpawnPoint
    {
        int posx, posy;
        size_t amount_to_spawn;
        float time_left_to_spawn;
    };

    inline static std::array<int, 12> s_Waves = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240};

private:
    void addSpawnPoint(size_t count, float delay);
    void requestSpawnLocation(int& posx, int& posy);
    void spawnBunch(SpawnPoint& point);
    bool continueSpawnWave(size_t count);

    std::vector<SpawnPoint> m_SpawnPoints;
    std::random_device m_RandomDevice;
    std::default_random_engine m_RandomEngine;
    class Game* m_Parent;

    size_t m_WaveCounter = 1;
    int m_EnemiesToSpawn = 0;
    int m_Height, m_Width;
    float m_TimeSinceLastSpawn = 0.f;
    const int m_SpawnCountCoef = 2;
    float m_SpawnPointDelay = 1.f;
    bool m_IsSpawning = false;

public:
    WaveSpawner() = delete;
    WaveSpawner(Game* parent, int height, int width);
    void startWave();
    void update(float delta);
    bool isCurrentlySpawning() const
    {
        return m_IsSpawning;
    }
    float getTimeSinceLastSpawn() const
    {
        return m_TimeSinceLastSpawn;
    }
};
