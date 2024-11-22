#include <command/command.h>

/**
 * Executes a command.
 */
void c_execute(c_command command)
{
    switch (command)
    {
    /* Player only commands. */
    case CS_FORWARD:
    case CE_FORWARD:
    case CS_BACKWARD:
    case CE_BACKWARD:
    case CS_RIGHT:
    case CE_RIGHT:
    case CS_LEFT:
    case CE_LEFT:
    case CS_JUMP:
    case CE_JUMP:
    case CS_PRIMARY_ATTACK:
    case CE_PRIMARY_ATTACK:
    case CS_SECONDARY_ATTACK:
    case CE_SECONDARY_ATTACK:
        DEBUG_ASSERT (
            "Tried to execute a player only command without a player"
        );
        break;
    }
}
