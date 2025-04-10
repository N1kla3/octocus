#include "game.h"
#include <chrono>
#include "AiComponents.h"
#include "AiSystem.h"
#include "AttackSystem.h"
#include "BorderComponent.h"
#include "CollisionSystem.h"
#include "DamageSystem.h"
#include "DrawSystem.h"
#include "InputSystem.h"
#include "KillSystem.h"
#include "LifeComponents.h"
#include "MovementSystem.h"
#include "RenderComponent.h"
#include "SpaceComponents.h"
#include "StatusComponents.h"
#include "WeaponComponent.h"
#include "networking/Serialization.h"
#include "raylib.h"

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
    namespace sc = std::chrono;
    auto begin = sc::steady_clock::now();

    std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(1000);
    ReadStream stream(std::move(buffer));

    std::unique_ptr<Buffer> sec_buffer = std::make_unique<Buffer>(1000);
    WriteStream write_stream(std::move(sec_buffer));

    int var = 32;
    write_stream.serialize(var);

    std::vector<float> floats{3.f, 5.f, 6.f};
    write_stream.serialize(floats);
    write_stream.serialize(var);

    while (!WindowShouldClose())
    {
        const auto current = sc::steady_clock::now();
        float const duration = sc::duration<float>{current - begin}.count();

        update(duration);

        begin = current;
    }

    tearDown();
}

void Game::update(const float delta)
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
    const auto entity = m_Registry.create();
    m_Registry.emplace<Health>(entity, param.health, param.health);
    m_Registry.emplace<Position>(entity, param.posx, param.posy);
    m_Registry.emplace<Velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<SphereCollision>(entity, param.size, CollisionChannel::BOT);
    m_Registry.emplace<RenderComponent>(entity, PURPLE, param.size, RenderPriority::MIDLE);
    m_Registry.emplace<WeaponComponent>(entity, param.damage, 10.f, 1.f);
    m_Registry.emplace<Damage>(entity);
    m_Registry.emplace<Bot>(entity);
    m_Registry.emplace<MeleeAi>(entity);
    m_Status.enemies_left++;
}

void Game::spawnRange(const SpawnParam& param)
{
    const auto entity = m_Registry.create();
    m_Registry.emplace<Health>(entity, param.health, param.health);
    m_Registry.emplace<Position>(entity, param.posx, param.posy);
    m_Registry.emplace<Velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<SphereCollision>(entity, param.size, CollisionChannel::BOT);
    m_Registry.emplace<RenderComponent>(entity, BLUE, param.size, RenderPriority::MIDLE);
    m_Registry.emplace<ShootComponent>(entity, param.damage, 1.4f);
    m_Registry.emplace<Damage>(entity);
    m_Registry.emplace<Bot>(entity);
    m_Registry.emplace<RangeAi>(entity);
    m_Status.enemies_left++;
}

void Game::updateGameplay(const float delta)
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

void Game::updateDrawFrame(const float delta)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawSystem::update(m_Registry, delta, m_Status);

    EndDrawing();
}

void Game::createPlayer()
{
    const auto entity = m_Registry.create();
    m_Registry.emplace<Player>(entity); // similar like tag, alternative
    m_Registry.emplace<RenderComponent>(entity, ORANGE, 10.f);
    m_Registry.emplace<Health>(entity, 100, 100);
    m_Registry.emplace<Position>(entity, 20.f, 20.f);
    m_Registry.emplace<Velocity>(entity, 0.f, 0.f);
    m_Registry.emplace<WeaponComponent>(entity, 60.f, 15.f, 1.3f);
    m_Registry.emplace<ShootComponent>(entity, 40.f, 0.3f, 0.0f);
    m_Registry.emplace<SphereCollision>(entity, 10.f, CollisionChannel::PLAYER);
    m_Registry.emplace<CollisionResolver>(entity);
    m_Registry.emplace<Damage>(entity);
    m_Registry.emplace<CameraTarget>(entity);
}

void Game::spawnBorders()
{
    const auto left = m_Registry.create();
    m_Registry.emplace<Border>(
            left, Vector2{0.f - 1000.f, 0.f - (2 * m_ScreenHeight)}, Vector2{0.f, 0.f + (2 * m_ScreenHeight)}, BROWN);

    const auto right = m_Registry.create();
    m_Registry.emplace<Border>(right,
                               Vector2{m_ScreenWidth, m_ScreenHeight + 1000.f},
                               Vector2{m_ScreenWidth + 1000.f, 0.f - 1000.f},
                               BROWN);

    const auto top = m_Registry.create();
    m_Registry.emplace<Border>(top, Vector2{0.f, 0.f}, Vector2{m_ScreenWidth, -m_ScreenHeight}, BROWN);

    const auto bottom = m_Registry.create();
    m_Registry.emplace<Border>(
            bottom, Vector2{0.f, 0.f + (2 * m_ScreenHeight)}, Vector2{m_ScreenWidth, m_ScreenHeight}, BROWN);

    Rectangle rect{};
    rect.x = 0.f;
    rect.y = 0.f;
    rect.width = m_ScreenWidth;
    rect.height = m_ScreenHeight;
}
