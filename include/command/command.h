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
    CS_FORWARD,
    CE_FORWARD,

    CS_BACKWARD,
    CE_BACKWARD,

    CS_LEFT,
    CE_LEFT,

    CS_RIGHT,
    CE_RIGHT,

    CS_JUMP,
    CE_JUMP,

    CS_PRIMARY_ATTACK,
    CE_PRIMARY_ATTACK,

    CS_SECONDARY_ATTACK,
    CE_SECONDARY_ATTACK,
} c_command;

/**
 * Executes a command.
 */
void c_execute(c_command command);

#endif
