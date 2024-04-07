#ifndef INPUT_MAP_MOUSE_H
#define INPUT_MAP_MOUSE_H

/* Holds input from a mouse. */
typedef struct i_mouse {
    /* The position of the mouse on the window. */
    f32_v2 pos;

    /* The amount scrolled. */
    f32 scroll_delta;

    /* The buttons on the mouse currently pressed. */
    bool i_mouse_button[I_MOUSE_BUTTONS];

    /* If the mouse is currently over the window. */
    bool active;
} i_mouse;

#endif
