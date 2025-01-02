#include <input/mouse.h>

/* The default maps for mouse commands. */
static const c_command i_mouse_default_press_map[I_MOUSE_BUTTONS] = {
    [I_MOUSE_BUTTON_LEFT_CLICK] = CS_PRIMARY_ATTACK,
    [I_MOUSE_BUTTON_RIGHT_CLICK] = CS_SECONDARY_ATTACK,
    [I_MOUSE_BUTTON_MIDDLE_CLICK] = C_NONE,
};

static const c_command i_mouse_default_hold_map[I_MOUSE_BUTTONS] = {
    [I_MOUSE_BUTTON_LEFT_CLICK] = C_NONE,
    [I_MOUSE_BUTTON_RIGHT_CLICK] = C_NONE,
    [I_MOUSE_BUTTON_MIDDLE_CLICK] = C_NONE,
};

static const c_command i_mouse_default_unpress_map[I_MOUSE_BUTTONS] = {
    [I_MOUSE_BUTTON_LEFT_CLICK] = CE_PRIMARY_ATTACK,
    [I_MOUSE_BUTTON_RIGHT_CLICK] = CE_SECONDARY_ATTACK,
    [I_MOUSE_BUTTON_MIDDLE_CLICK] = C_NONE,
};

/**
 * Saves the current input from a mouse into the last input of the mouse.
 */
void i_mouse_save_last_buttons(i_mouse* mouse)
{
    memcpy(mouse->last_buttons, mouse->buttons, sizeof(mouse->buttons));
}

/**
 * Loads in the default map into a mouse input processor.
 */
void i_mouse_load_default(i_mouse* mouse)
{
    memcpy (
        &mouse->press_map,
        i_mouse_default_press_map,
        sizeof(mouse->press_map)
    );

    memcpy (
        &mouse->hold_map,
        i_mouse_default_hold_map,
        sizeof(mouse->hold_map)
    );

    memcpy (
        &mouse->unpress_map,
        i_mouse_default_unpress_map,
        sizeof(mouse->unpress_map)
    );
}
