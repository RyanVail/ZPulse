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

    case CS_FORWARD:
    case CS_BACKWARD:
    case CS_LEFT:
    case CS_RIGHT:
        player->move_input[(command - CS_FORWARD) / 2] = 1.0f;
        break;

    case CE_FORWARD:
    case CE_BACKWARD:
    case CE_LEFT:
    case CE_RIGHT:
        player->move_input[(command - CE_FORWARD) / 2] = 0.0f;
        break;

    // TODO: Player jumping needs somekind of delay for holding down the
    // button.
    case CS_JUMP:
        player->jumping = true;
        break;
    case CE_JUMP:
        player->jumping = false;
        break;

    case CS_PRIMARY_ATTACK:
        player->primary_attacking = true;
        break;
    case CE_PRIMARY_ATTACK:
        player->primary_attacking = false;
        break;

    case CS_SECONDARY_ATTACK:
        player->secondary_attacking = true;
        break;
    case CE_SECONDARY_ATTACK:
        player->secondary_attacking = false;
        break;
    }
}
