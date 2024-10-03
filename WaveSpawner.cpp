#include "WaveSpawner.h"
#include "game.h"
#include <random>

void WaveSpawner::addSpawnPoint(size_t count, float delay)
{
    int x, y;
    requestSpawnLocation(x, y);
    m_SpawnPoints.emplace_back(x, y, count, delay);
    
    // TODO: Add animation of pre-spawn
}

void WaveSpawner::requestSpawnLocation(int& posx, int& posy)
{
    std::uniform_int_distribution<int> dist(0, m_Width - 30); 
    std::uniform_int_distribution<int> dist_y(0, m_Height - 30); 
    posx = dist(m_RandomEngine);
    posy = dist_y(m_RandomEngine); 
}

void WaveSpawner::spawnBunch(SpawnPoint& point)
{
    std::poisson_distribution<int> dist(16); 
    std::poisson_distribution<int> dist_y(16); 
    std::uniform_int_distribution<int> type_randomizer(0,1);
    for (size_t i = 0; i < point.amount_to_spawn; i++)
    {
        SpawnParam params{};
        params.posx = point.posx + dist(m_RandomEngine);
        params.posy = point.posy + dist_y(m_RandomEngine);
        params.size = 10.f;
        params.damage = 10.f;
        params.health = 20;

        if (type_randomizer(m_RandomEngine))
        {
            m_Parent->spawnMelee(params);
        }
        else
        {
            m_Parent->spawnRange(params);
        }
    }
    m_TimeSinceLastSpawn = 0.f;
}

bool WaveSpawner::continueSpawnWave(size_t count)
{
    m_EnemiesToSpawn -= count;
    bool spawn = m_EnemiesToSpawn >= 0;
    if (spawn)
    {
        m_SpawnPointDelay += 0.2f;
        addSpawnPoint(count, m_SpawnPointDelay);
    }
    return spawn;
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
    m_SpawnPointDelay = 1.f;
    if (m_WaveCounter < s_Waves.size())
    {
        m_EnemiesToSpawn = s_Waves[m_WaveCounter];
        m_IsSpawning = true;
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
    m_IsSpawning = !m_SpawnPoints.empty();
}

