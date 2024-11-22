#ifndef RENDER_CAMERA_H
#define RENDER_CAMERA_H

#include <math/f32_v2.h>

/* A camera used for rendering. */
typedef struct r_cam {
    /* The position fo the camera in the game. */
    f32_v2 pos;

    /* The zoom of the camera. */
    f32_v2 zoom;
} r_cam;

/**
 * Converts a position within a camera to a position within the game.
 */
f32_v2 r_cam_pos_to_game_pos(const r_cam* cam, const f32_v2 pos);

#endif
