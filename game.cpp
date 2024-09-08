#include "game.h"
#include "CollisionSystem.h"
#include "raylib.h"
#include "SpaceComponents.h"
#include <iostream>
#include "RenderComponent.h"
#include "DamageSystem.h"
#include "KillSystem.h"
#include "AttackSystem.h"
#include "LifeComponents.h"
#include "WeaponComponent.h"
#include "DrawSystem.h"
#include "MovementSystem.h"
#include "InputSystem.h"
#include <chrono>

Game::Game()
    : m_Spawner(this, m_ScreenHeight, m_ScreenWidth)
    , m_Registry()
{
    
}

void Game::init()
{
    InitWindow(m_ScreenWidth, m_ScreenHeight, "Octocus");

    SetTargetFPS(120);

    createPlayer();
}

void Game::run()
{
    using namespace std::chrono;
    auto begin = steady_clock::now();

    while (!WindowShouldClose())
    {
        const auto current = steady_clock::now();
        float duration = std::chrono::duration<float>{current - begin}.count();

        update(duration);

        begin = current;
    }

    tearDown();
}

void Game::update(float delta)
{
    updateGameplay(delta);
    updateDrawFrame(delta);
}

void Game::tearDown()
{
    CloseWindow();
}

void Game::spawnMelee(const SpawnParam& param)
{
    auto entity = m_Registry.create();
    m_Registry.emplace<Health>(entity, param.health, param.health);
    m_Registry.emplace<position>(entity, param.posx, param.posy);
    m_Registry.emplace<velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<sphere_collision>(entity, param.size);
    m_Registry.emplace<RenderComponent>(entity, BLUE, param.size, RenderPriority::MIDLE);
    m_Registry.emplace<WeaponComponent>(entity, param.damage, 10.f, 2.f, true);
    std::cout << "spawned melee at pos" << param.posx << " " << param.posy << std::endl;
}

void Game::spawnRange(const SpawnParam& param)
{
    auto entity = m_Registry.create();
    m_Registry.emplace<Health>(entity, param.health, param.health);
    m_Registry.emplace<position>(entity, param.posx, param.posy);
    m_Registry.emplace<velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<sphere_collision>(entity, param.size);
    m_Registry.emplace<RenderComponent>(entity, BLUE, param.size, RenderPriority::MIDLE);
    m_Registry.emplace<ShootComponent>(entity, param.damage, 10.f, 0.f);
}

void Game::updateGameplay(float delta)
{
    if (!m_Spawner.isCurrentlySpawning() && m_Spawner.getTimeSinceLastSpawn() > 3.f)
    {
        m_Spawner.startWave();
    }
    m_Spawner.update(delta);

    InputSystem::update(m_Registry, delta);
    MovementSystem::update(m_Registry, delta);
    CollisionSystem::update(m_Registry);
    AttackSystem::update(m_Registry);
    DamageSystem::updateDamage(m_Registry);
    KillSystem::update(m_Registry);
    return;
    auto view = m_Registry.view<const position, velocity>();

    // use a callback
    view.each([](const auto &pos, auto &vel) { /* ... */ });

    // use an extended callback
    view.each([](const auto entity, const auto &pos, auto &vel) { /* ... */ });

    // use a range-for
    for(auto [entity, pos, vel]: view.each()) {
        // ...
    }

    // use forward iterators and get only the components of interest
    for(auto entity: view) {
        auto &vel = view.get<velocity>(entity);
        // ...
    }
}

void Game::updateDrawFrame(float delta)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawSystem::update(m_Registry, delta);

    //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

void Game::createPlayer()
{
    auto entity = m_Registry.create();
    m_Registry.emplace<player>(entity); // similar like tag, alternative
    m_Registry.emplace<RenderComponent>(entity, ORANGE, 10.f);
    m_Registry.emplace<Health>(entity, 100.f, 100.f);
    m_Registry.emplace<position>(entity, 20.f, 20.f);
    m_Registry.emplace<velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<WeaponComponent>(entity, 10.f, 10.f, 0.4f, false);
    m_Registry.emplace<ShootComponent>(entity, 10.f, 10.f, 0.0f);
    m_Registry.emplace<sphere_collision>(entity, 10.f);
    m_Registry.emplace<collision_resolver>(entity);
}

