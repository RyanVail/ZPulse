#ifndef RENDER_INIT_H
#define RENDER_INIT_H

/* Possible errors encounted during initialization. */
typedef enum r_init_error {
    /* No error. */
    R_INIT_ERROR_NONE = 0,

    /* Failed to create a window. */
    R_INIT_ERROR_WINDOW,
} r_init_error;

/**
 * Inits the renderer.
 *
 * @return An error code.
 */
r_init_error r_init();

/**
 * Unitializes the renderer.
 */
void r_uninit();

#endif
