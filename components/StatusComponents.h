#pragma once


struct GameStatus
{
    bool is_player_dead = false;
    bool wave_finished = false;
    unsigned long score = 0;
};
