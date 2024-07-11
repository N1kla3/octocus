#include "WaveSpawner.h"
#include "Game.h"
#include <random>

void WaveSpawner::spawnEnemies(entt::registry& registry, size_t count)
{
    int x, y;
    requestSpawnLocation(x, y);
    m_SpawnPoints.emplace_back(x, y, count, 1.f);
    
    // TODO: Add animation of pre-spawn
}

void WaveSpawner::requestSpawnLocation(int& posx, int& posy)
{
}

void WaveSpawner::spawnBunch(SpawnPoint& point)
{

}

WaveSpawner::WaveSpawner(Game* parent, int height, int width)
    : m_Parent(parent)
    , m_Height(height)
    , m_Width(width)
{

}

void WaveSpawner::startWave(entt::registry& registry)
{
    if (m_WaveCounter < s_Waves.size())
    {
        m_EnemiesToSpawn = s_Waves[m_WaveCounter];
        spawnEnemies(registry, m_EnemiesToSpawn);
        m_EnemiesToSpawn -= s_Waves[m_WaveCounter];
    }
}

void WaveSpawner::update(float delta)
{
    for (auto& point : m_SpawnPoints)
    {
        spawnBunch(point);
    }
}

