#ifndef INPUT_INPUT_H
#define INPUT_INPUT_H

#include <common.h>
#include <input/keyboard.h>
#include <input/mouse.h>
#include <input/controller.h>

/* Types of input processors. */
typedef enum i_type {
    I_KEYBOARD_AND_MOUSE,
    I_CONTROLLER,
} i_type;

/* An input processor. */
typedef struct i_input {
    /* The type of this input processor. */
    i_type type;

    /* The input processor. */
    union {
        struct {
            i_keyboard keyboard;
            i_mouse mouse;
        } keyboard_and_mouse;
        i_controller controller;
    } processor;
} i_input;

/**
 * Reads input from an input processor.
 */
void i_read(i_input* processor);

#endif
