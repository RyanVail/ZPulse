#include <player/input.h>
#include <player/command.h>
#include <debug/debug.h>

// TODO: Make a version of this for mouse and controller.
/**
 * Gets the keyboard input processor of a player. The player must have a
 * keyboard and mouse input type.
 */
i_keyboard* p_keyboard(p_player* player)
{
    DEBUG_ASSERT (player->input.type == I_KEYBOARD_AND_MOUSE,
        "Tried to get the keyboard input of a player that doesn't have a "
        "keyboard."
    );

    return &player->input.processor.keyboard_and_mouse.keyboard;
}

/**
 * Processes the input from a player's keyboard by executing the commands
 * assigned to each input.
 */
void p_keyboard_execute(p_player* player)
{
    const i_keyboard* keyboard = p_keyboard(player);
    for (i_key key = 0; key < I_KEY_ENUM_MAX; key++)
        if (keyboard->keys[(size_t)key])
            p_execute(player, keyboard->map[(size_t)key]);
}

/**
 * Reads and proccesses this player's input.
 */
void p_input(p_player* player)
{
    /* Clearing the old move input. */
    memset(player->move_input, 0, sizeof(player->move_input));

    /* Reading the player's input. */
    i_read(&player->input);

    /* Processing the input. */
    switch (player->input.type)
    {
    case I_KEYBOARD_AND_MOUSE:
        p_keyboard_execute(player);
        break;
    case I_CONTROLLER:
        // TODO: Implement controller here.
        break;

    default:
        UNREACHABLE();
    }
}

/**
 * Gets the normalized version of a player's move direction.
 */
f32_v2 p_normalize_move_input(const p_player* player)
{
    /* Combinding the opposite move directions. */
    f32_v2 move = {
        player->move_input[2] - player->move_input[3],
        player->move_input[0] - player->move_input[1],
    };

    // TODO: This won't allow controller players to walk very slowly forward,
    // account for that.
    /* Normalizing the result to keep the movement consistant. */
    return f32_v2_normalize(move);
}
