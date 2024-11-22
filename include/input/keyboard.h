#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H

#include <common.h>
#include <command/command.h>

// TODO: Implement more keys.
typedef enum i_key {
    I_KEY_A,
    I_KEY_B,
    I_KEY_C,
    I_KEY_D,
    I_KEY_E,
    I_KEY_F,
    I_KEY_G,
    I_KEY_H,
    I_KEY_I,
    I_KEY_J,
    I_KEY_K,
    I_KEY_L,
    I_KEY_M,
    I_KEY_N,
    I_KEY_O,
    I_KEY_P,
    I_KEY_Q,
    I_KEY_R,
    I_KEY_S,
    I_KEY_T,
    I_KEY_U,
    I_KEY_V,
    I_KEY_W,
    I_KEY_X,
    I_KEY_Y,
    I_KEY_Z,

    #define I_KEYS (I_KEY_Z + 1)
} i_key;

/* Holds input from a keyboard. */
typedef struct i_keyboard {
    /* Commands that activate when keys are first pressed. */
    c_command press_map[I_KEYS];

    /* Commands that activate when keys are being pressed. */
    c_command hold_map[I_KEYS];

    /* Commands that activate when keys stop being pressed. */
    c_command unpress_map[I_KEYS];

    /* The buttons on the keyboard currently being pressed. */
    bool keys[I_KEYS];

    /* The buttons on the keyboard that were being pressed the last poll. */
    bool last_keys[I_KEYS];
} i_keyboard;

/**
 * Reads input from a keyboard.
 */
void i_keyboard_read(i_keyboard* keyboard);

/**
 * Saves the current input from a keyboard into the last input of the
 * keyboard.
 */
void i_keyboard_save_last_keys(i_keyboard* keyboard);

/**
 * Loads in the default map into a keyboard input processor.
 */
void i_keyboard_load_default(i_keyboard* keyboard);

#endif
