#include <player/input.h>
#include <player/command.h>
#include <debug/debug.h>

// TODO: Make a version of this for controller.
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
 * Gets the keyboard input processor of a player. The player must have a
 * keyboard and mouse input type.
 */
i_mouse* p_mouse(p_player* player)
{
    DEBUG_ASSERT (player->input.type == I_KEYBOARD_AND_MOUSE,
        "Tried to get the mouse input of a player that doesn't have a "
        "mouse."
    );

    return &player->input.processor.keyboard_and_mouse.mouse;
}

/**
 * Processes the input from a player's keyboard by executing the commands
 * assigned to each input.
 */
void p_keyboard_execute(p_player* player)
{
    const i_keyboard* keyboard = p_keyboard(player);
    for (i_key key = 0; key < I_KEYS; key++) {
        const bool pressed = keyboard->keys[(size_t)key];
        const bool last_pressed = keyboard->last_keys[(size_t)key];

        if (pressed && last_pressed)
            p_execute(player, keyboard->press_map[(size_t)key]);
        else if (pressed && !last_pressed)
            p_execute(player, keyboard->hold_map[(size_t)key]);
        else if (last_pressed)
            p_execute(player, keyboard->unpress_map[(size_t)key]);
    }
}

/**
 * Processes the input from a player's mouse by executing the commands assigned
 * to each input.
 */
void p_mouse_execute(p_player* player)
{
    const i_mouse* mouse = p_mouse(player);
    for (i_mouse_button b = 0; b < I_MOUSE_BUTTONS; b++) {
        const bool pressed = mouse->buttons[(size_t)b];
        const bool last_pressed = mouse->last_buttons[(size_t)b];

        if (pressed && last_pressed)
            p_execute(player, mouse->press_map[(size_t)b]);
        else if (pressed && !last_pressed)
            p_execute(player, mouse->hold_map[(size_t)b]);
        else if (last_pressed)
            p_execute(player, mouse->unpress_map[(size_t)b]);
    }
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
        p_mouse_execute(player);
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
