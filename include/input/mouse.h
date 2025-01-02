#ifndef INPUT_MOUSE_H
#define INPUT_MOUSE_H

#include <math/f32_v2.h>
#include <command/command.h>
#include <assert.h>

typedef enum i_mouse_button {
    I_MOUSE_BUTTON_LEFT_CLICK,
    I_MOUSE_BUTTON_RIGHT_CLICK,
    I_MOUSE_BUTTON_MIDDLE_CLICK,
} i_mouse_button;

static_assert ((i32)((i_mouse_button)I_MOUSE_BUTTONS) == (i32)I_MOUSE_BUTTONS,
    "i_mouse_button is not of a large enough int type to hold I_MOUSE_BUTTONS."
);

/* Holds input from a mouse. */
typedef struct i_mouse {
    /* Commands that activate when mouse buttons are first pressed. */
    c_command press_map[I_MOUSE_BUTTONS];

    /* Commands that activate when mouse buttons are being pressed. */
    c_command hold_map[I_MOUSE_BUTTONS];

    /* Commands that activate when mouse buttons stop being pressed. */
    c_command unpress_map[I_MOUSE_BUTTONS];

    /* The position of the mouse on the window. */
    f32_v2 pos;

    /* The amount scrolled. */
    f32 scroll_delta;

    /* The buttons on the mouse currently being pressed. */
    bool buttons[I_MOUSE_BUTTONS];

    /* The buttons on the mouse that were being pressed last poll. */
    bool last_buttons[I_MOUSE_BUTTONS];

    /* If the mouse is currently over the window. */
    bool active;
} i_mouse;

/**
 * Reads input from a mouse.
 */
void i_mouse_read(i_mouse* mouse);

/**
 * Saves the current input from a mouse into the last input of the mouse.
 */
void i_mouse_save_last_buttons(i_mouse* mouse);

/**
 * Loads in the default map into a mouse input processor.
 */
void i_mouse_load_default(i_mouse* mouse);

#endif
