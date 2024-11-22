#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <player/player.h>

/**
 * Gets the keyboard input processor of a player. The player must have a
 * keyboard and mouse input type.
 */
i_keyboard* p_keyboard(p_player* player);

/**
 * Gets the keyboard input processor of a player. The player must have a
 * keyboard and mouse input type.
 */
i_mouse* p_mouse(p_player* player);

/**
 * Reads and proccesses this player's input.
 */
void p_input(p_player* player);

/**
 * Gets the normalized version of a player's move input direction.
 */
f32_v2 p_normalize_move_input(const p_player* player);

#endif
