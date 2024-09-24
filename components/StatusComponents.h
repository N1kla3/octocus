#pragma once

struct GameStatus
{
    bool is_player_dead = false;
    bool is_paused = false;
    bool request_restart = false;
};
