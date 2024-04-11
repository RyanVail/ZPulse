#include <input/keyboard.h>

/* The default map for keyboard commands. */
static const c_command i_keyboard_default_map[I_KEY_ENUM_MAX] = {
    [I_KEY_W] = C_FORWARD,
    [I_KEY_S] = C_BACKWARD,
    [I_KEY_A] = C_RIGHT,
    [I_KEY_D] = C_LEFT,
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
    memcpy(&keyboard->map, i_keyboard_default_map, sizeof(keyboard->map));
}
