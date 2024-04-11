#ifndef COMMANDS_COMMANDS_H
#define COMMANDS_COMMANDS_H

#include <common.h>

// TODO: There could be flags for toggling a command and stuff like that.
// TODO: Commands should be able to come with args so that for moving a
// controller can just move the player a little.
/* A command. */
typedef enum PACKED c_command {
    /* No command. */
    C_NONE = 0,

    /* Player only commands. */
    C_FORWARD,
    C_BACKWARD,
    C_LEFT,
    C_RIGHT,
    C_JUMP,
} c_command;

/**
 * Executes a command.
 */
void c_execute(c_command command);

#endif
