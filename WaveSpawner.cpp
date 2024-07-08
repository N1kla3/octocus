#include "WaveSpawner.h"
#include "LifeComponents.h"
#include "SpaceComponents.h"
#include "RenderComponent.h"
#include "raylib.h"
#include "WeaponComponent.h"

void WaveSpawner::spawnRangeEnemy(entt::registry& registry)
{
    auto entity = registry.create();
    registry.emplace<Health>(entity, 10, 10);
    registry.emplace<position>(entity, 10.f, 10.f);
    registry.emplace<velocity>(entity, 0.f, 0.f);
    registry.emplace<sphere_collision>(entity, 10.f);
    registry.emplace<RenderComponent>(entity, BLUE, 10.f, RenderPriority::MIDLE);
    registry.emplace<WeaponComponent>(entity, 10.f, 10.f, 2.f, true);

}

void WaveSpawner::spawnMeleeEnemy(entt::registry& registry)
{
    auto entity = registry.create();
    registry.emplace<Health>(entity, 10, 10);
    registry.emplace<position>(entity, 10.f, 10.f);
    registry.emplace<velocity>(entity, 0.f, 0.f);
    registry.emplace<sphere_collision>(entity, 10.f);
    registry.emplace<RenderComponent>(entity, BLUE, 10.f, RenderPriority::MIDLE);
}

void WaveSpawner::startWave(entt::registry& registry)
{
    if (m_WaveCounter < s_Waves.size())
    {
        m_EnemiesToSpawn = s_Waves[m_WaveCounter];
        spawnRangeEnemy(registry);
        spawnMeleeEnemy(registry);
    }
}

void WaveSpawner::update(float delta)
{

}
