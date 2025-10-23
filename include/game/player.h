#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <player/player.h>

// TODO: There has to be a way to remove players too.
/**
 * Adds a player to the global local player list.
 */
p_player* g_add_player();

/**
 * Ticks all players in the local player list.
 */
void g_tick_players();

#endif
