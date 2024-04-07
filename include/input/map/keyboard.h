#ifndef INPUT_MAP_KEYBOARD_H
#define INPUT_MAP_KEYBOARD_H

#include <common.h>

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

    I_KEY_ENUM_MAX,
} i_key;

/* Holds input from a keyboard. */
typedef struct i_keyboard {
    /* The buttons on the keyboard currently being pressed. */
    bool keys[I_KEY_ENUM_MAX];
} i_keyboard;

#endif
