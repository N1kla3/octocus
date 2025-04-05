#pragma once
#include "StatusComponents.h"
#include "WaveSpawner.h"
#include "entt/entt.hpp"

struct SpawnParam
{
    float posx, posy;
    int health;
    float size;
    float damage;
};

class Game final
{
public:
    Game(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game& operator=(Game&& other) = delete;

public:
    Game();
    ~Game() = default;

    void init();
    void run();
    void tearDown();
    void spawnMelee(const SpawnParam& param);
    void spawnRange(const SpawnParam& param);
    inline static const int m_ScreenWidth = 800;
    inline static const int m_ScreenHeight = 450;

private:
    WaveSpawner m_Spawner;
    entt::registry m_Registry;

    GameStatus m_Status;
    float m_WaveDelay = 0.f;


    void update(float delta);
    void updateGameplay(float delta);
    void updateDrawFrame(float delta);

    void createPlayer();
    void spawnBorders();
};
