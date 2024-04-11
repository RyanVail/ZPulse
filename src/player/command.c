#include <player/command.h>
#include <debug/debug.h>

/**
 * Executes a command on a player.
 *
 * @param The player cannot be NULL.
 * @param command The command to execute on the player.
 */
void p_execute(p_player* player, c_command command)
{
    DEBUG_ASSERT (player != NULL,
        "Tried to execute a command on a NULL player."
    );

    switch (command)
    {
    case C_NONE:
        break;

    case C_FORWARD:
    case C_BACKWARD:
    case C_LEFT:
    case C_RIGHT:
        player->move_input[command - C_FORWARD] = 1.0f;
        break;

    // TODO: Player jumping needs somekind of delay for holding down the
    // button.
    case C_JUMP:
        player->jump = true;
        break;
    }
}
