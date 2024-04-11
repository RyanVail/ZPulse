#include <command/command.h>

/**
 * Executes a command.
 */
void c_execute(c_command command)
{
    switch (command)
    {
    /* Player only commands. */
    case C_FORWARD:
    case C_BACKWARD:
    case C_RIGHT:
    case C_LEFT:
    case C_JUMP:
        DEBUG_ASSERT (
            "Tried to execute a player only command without a player"
        );
        break;
    }
}
