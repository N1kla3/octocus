#pragma once
#include <array>
#include "entt/entt.hpp"

class WaveSpawner
{
    struct SpawnPoint
    {
        int posx, posy;
        size_t amount_to_spawn;
        float time_left_to_spawn;
    };

    inline static std::array<int, 12> s_Waves = { 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240 };
private:
    void spawnEnemies(entt::registry& registry, size_t count);
    void requestSpawnLocation(int& posx, int& posy);
    void spawnBunch(SpawnPoint& point);

    std::vector<SpawnPoint> m_SpawnPoints;
    class Game* m_Parent;
    size_t m_WaveCounter = 0;
    int m_EnemiesToSpawn = 0;
    int m_Height, m_Width;
    float time_since_lastspawn = 0.f;

public:
    WaveSpawner() = delete;
    WaveSpawner(Game* parent, int height, int width);
    void startWave(entt::registry& registry);
    void update(float delta);
};

