#pragma once 
#include "entt/entt.hpp"
#include "WaveSpawner.h"

struct SpawnParam
{
    int posx, posy;
    int health;
    float size;
    float damage;
};

class Game final
{
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
private:
    WaveSpawner m_Spawner;
    entt::registry m_Registry;

    inline static const int m_ScreenWidth = 800;
    inline static const int m_ScreenHeight = 450;

    void update(float delta);
    void updateGameplay(float delta);
    void updateDrawFrame(float delta);
};
