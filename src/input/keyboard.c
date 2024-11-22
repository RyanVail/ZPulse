#include <input/keyboard.h>

/* The default map for keyboard commands. */
static const c_command i_keyboard_default_press_map[I_KEYS] = {
    [I_KEY_W] = CS_FORWARD,
    [I_KEY_S] = CS_BACKWARD,
    [I_KEY_A] = CS_RIGHT,
    [I_KEY_D] = CS_LEFT,
};

static const c_command i_keyboard_default_hold_map[I_KEYS] = {
    [I_KEY_W] = C_NONE,
    [I_KEY_S] = C_NONE,
    [I_KEY_A] = C_NONE,
    [I_KEY_D] = C_NONE,
};

static const c_command i_keyboard_default_unpress_map[I_KEYS] = {
    [I_KEY_W] = CE_FORWARD,
    [I_KEY_S] = CE_BACKWARD,
    [I_KEY_A] = CE_RIGHT,
    [I_KEY_D] = CE_LEFT,
};

/**
 * Saves the current input from a keyboard into the last input of the
 * controller.
 */
void i_keyboard_save_last_keys(i_keyboard* keyboard)
{
    memcpy(keyboard->last_keys, keyboard->keys, sizeof(keyboard->keys));
}

/**
 * Loads in the default map into a keyboard input processor.
 */
void i_keyboard_load_default(i_keyboard* keyboard)
{
    memcpy (
        &keyboard->press_map,
        i_keyboard_default_press_map,
        sizeof(keyboard->press_map)
    );

    memcpy (
        &keyboard->hold_map,
        i_keyboard_default_hold_map,
        sizeof(keyboard->hold_map)
    );

    memcpy (
        &keyboard->unpress_map,
        i_keyboard_default_unpress_map,
        sizeof(keyboard->unpress_map)
    );
}
