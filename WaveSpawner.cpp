#include "WaveSpawner.h"
#include "game.h"
#include <random>

void WaveSpawner::addSpawnPoint(size_t count)
{
    int x, y;
    requestSpawnLocation(x, y);
    m_SpawnPoints.emplace_back(x, y, count, 1.f);
    
    // TODO: Add animation of pre-spawn
}

void WaveSpawner::requestSpawnLocation(int& posx, int& posy)
{
    std::uniform_int_distribution<int> dist(0, m_Width); 
    std::uniform_int_distribution<int> dist_y(0, m_Height); 
    posx = dist(m_RandomEngine);
    posy = dist_y(m_RandomEngine); 
}

void WaveSpawner::spawnBunch(SpawnPoint& point)
{
    std::poisson_distribution<int> dist(16); 
    std::poisson_distribution<int> dist_y(16); 
    for (size_t i = 0; i < point.amount_to_spawn; i++)
    {
        SpawnParam params{};
        params.posx = point.posx + dist(m_RandomEngine);
        params.posy = point.posy + dist_y(m_RandomEngine);
        params.size = 10.f;
        params.damage = 10.f;
        params.health = 20;
        // TODO: melee range
        m_Parent->spawnMelee(params);
    }
    m_TimeSinceLastSpawn = 0.f;
}

bool WaveSpawner::continueSpawnWave(size_t count)
{
    m_EnemiesToSpawn -= count;
    m_IsSpawning = m_EnemiesToSpawn >= 0;
    if (m_IsSpawning)
    {
        addSpawnPoint(count);
    }
    return m_IsSpawning;
}

WaveSpawner::WaveSpawner(Game* parent, int height, int width)
    : m_Parent(parent)
    , m_Height(height)
    , m_Width(width)
    , m_RandomEngine(m_RandomDevice())
{
}

void WaveSpawner::startWave()
{
    if (m_WaveCounter < s_Waves.size())
    {
        m_EnemiesToSpawn = s_Waves[m_WaveCounter];
    }
    m_WaveCounter++;
    m_TimeSinceLastSpawn = 0.f;
}

void WaveSpawner::update(float delta)
{
    bool should_continue_spawn = continueSpawnWave(m_WaveCounter * m_SpawnCountCoef);
    if (m_SpawnPoints.empty() && !should_continue_spawn)
    {
        m_TimeSinceLastSpawn += delta;
        return;
    }

    for (auto it = m_SpawnPoints.begin(); it != m_SpawnPoints.end(); )
    {
        it->time_left_to_spawn -= delta;
        if (it->time_left_to_spawn <= 0.f)
        {
            spawnBunch(*it);
            it = m_SpawnPoints.erase(it);
        }
        else
        {
            it++;
        }
    }
}

