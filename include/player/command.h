#ifndef PLAYER_COMMANDS_H
#define PLAYER_COMMANDS_H

#include <player/player.h>
#include <command/command.h>

/**
 * Executes a command on a player.
 *
 * @param The player cannot be NULL.
 * @param command The command to execute on the player.
 */
void p_execute(p_player* player, c_command command);

#endif
