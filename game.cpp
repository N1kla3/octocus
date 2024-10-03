#include "game.h"
#include "AiComponents.h"
#include "AiSystem.h"
#include "BorderComponent.h"
#include "CollisionSystem.h"
#include "raylib.h"
#include "SpaceComponents.h"
#include "StatusComponents.h"
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

    spawnBorders();

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
    m_Registry.emplace<sphere_collision>(entity, param.size, CollisionChannel::BOT);
    m_Registry.emplace<RenderComponent>(entity, BLUE, param.size, RenderPriority::MIDLE);
    m_Registry.emplace<WeaponComponent>(entity, param.damage, 10.f, 2.f);
    m_Registry.emplace<Damage>(entity);
    m_Registry.emplace<bot>(entity);
    m_Registry.emplace<MeleeAi>(entity);
    m_Status.enemies_left++;
    //std::cout << "spawned melee at pos" << param.posx << " " << param.posy << std::endl;
}

void Game::spawnRange(const SpawnParam& param)
{
    auto entity = m_Registry.create();
    m_Registry.emplace<Health>(entity, param.health, param.health);
    m_Registry.emplace<position>(entity, param.posx, param.posy);
    m_Registry.emplace<velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<sphere_collision>(entity, param.size, CollisionChannel::BOT);
    m_Registry.emplace<RenderComponent>(entity, BLUE, param.size, RenderPriority::MIDLE);
    m_Registry.emplace<ShootComponent>(entity, param.damage, 1.4f);
    m_Registry.emplace<Damage>(entity);
    m_Registry.emplace<bot>(entity);
    m_Registry.emplace<RangeAi>(entity);
    m_Status.enemies_left++;
}

void Game::updateGameplay(float delta)
{
    if (!m_Spawner.isCurrentlySpawning() && m_Status.enemies_left == 0)
    {
        if (m_WaveDelay > 3.f)
        {
            m_Spawner.startWave();
            m_Status.wave_finished = true;
            m_WaveDelay = 0.f;
            m_Status.next_wave_timer = 0.f;
        }
        else if (!m_Spawner.isCurrentlySpawning())
        {
            m_WaveDelay += delta;
            m_Status.next_wave_timer = 3.f - m_WaveDelay;
        }
    }
    m_Spawner.update(delta);

    if (!m_Status.is_player_dead)
    {
        InputSystem::update(m_Registry, delta);
        AiSystem::updateAi(m_Registry, delta);
        MovementSystem::update(m_Registry, delta);
        CollisionSystem::update(m_Registry);
        AttackSystem::update(m_Registry, delta);
        DamageSystem::updateDamage(m_Registry);
        KillSystem::update(m_Registry, delta, m_Status);
    }
}

void Game::updateDrawFrame(float delta)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawSystem::update(m_Registry, delta, m_Status);

    EndDrawing();
}

void Game::createPlayer()
{
    auto entity = m_Registry.create();
    m_Registry.emplace<player>(entity); // similar like tag, alternative
    m_Registry.emplace<RenderComponent>(entity, ORANGE, 10.f);
    m_Registry.emplace<Health>(entity, 100, 100);
    m_Registry.emplace<position>(entity, 20.f, 20.f);
    m_Registry.emplace<velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<WeaponComponent>(entity, 44410.f, 15.f, 1.3f);
    m_Registry.emplace<ShootComponent>(entity, 55510.f, 0.5f, 0.0f);
    m_Registry.emplace<sphere_collision>(entity, 10.f, CollisionChannel::PLAYER);
    m_Registry.emplace<collision_resolver>(entity);
    m_Registry.emplace<Damage>(entity);
    m_Registry.emplace<cameraTarget>(entity);
}

void Game::spawnBorders()
{
    auto left = m_Registry.create();
    m_Registry.emplace<border>(left, Vector2{ 0.f - 1000.f, 0.f - 2 * m_ScreenHeight }, Vector2{ 0.f, 0.f + 2 * m_ScreenHeight }, BROWN);

    auto right = m_Registry.create();
    m_Registry.emplace<border>(right, Vector2{ m_ScreenWidth, m_ScreenHeight + 1000.f }, Vector2{ m_ScreenWidth + 1000.f, 0.f - 1000.f }, BROWN);

    auto top = m_Registry.create();
    m_Registry.emplace<border>(top, Vector2{ 0.f, 0.f }, Vector2{ m_ScreenWidth, -m_ScreenHeight }, BROWN);

    auto bottom = m_Registry.create();
    m_Registry.emplace<border>(bottom, Vector2{ 0.f, 0.f + 2 * m_ScreenHeight }, Vector2{ m_ScreenWidth, m_ScreenHeight }, BROWN);

    Rectangle&& rect{};
    rect.x = 0.f;
    rect.y = 0.f;
    rect.width = m_ScreenWidth;
    rect.height = m_ScreenHeight;
}

