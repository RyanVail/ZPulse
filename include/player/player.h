#ifndef PLAYER_PLAYER_H
#define PLAYER_PLAYER_H

#include <math/f32_v2.h>
#include <input/input.h>

// TODO: The void* should be defined by the game and should contain all other
// data a player should have not just the object of a player.
/* A basic player. */
typedef struct p_player {
    /* The object this player is bound to. */
    void* obj;

    /* The input processor of this player. */
    i_input input;

    /* The amount this player is trying to move in each direction. */
    f32 move_input[4];

    /* If this player is trying to jump. */
    bool jump;
} p_player;

/**
 * Ticks a player.
 */
void p_tick(p_player* player);

#endif
