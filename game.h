#pragma once 
#include "entt/entt.hpp"

class Game final
{
    Game(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game& operator=(Game&& other) = delete;

public:
    Game() = default;
    ~Game() = default;

    void init();
    void run();
    void tearDown();
private:
    entt::registry m_Registry;

    int m_ScreenWidth = 800;
    int m_ScreenHeight = 450;

    void update(float delta);
    void updateGameplay(float delta);
    void updateDrawFrame(float delta);
};
