#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <player/player.h>

/**
 * Inits the game.
 */
void g_init();

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
