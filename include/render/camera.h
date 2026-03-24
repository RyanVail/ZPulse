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
 * Calculates the inverse zoom of a camera.
 */
f32_v2 r_cam_inv_zoom(const r_cam* cam);

/**
 * Converts a game position to a camera's position.
 */
f32_v2 r_game_pos_to_cam_pos(const r_cam* cam, f32_v2 pos);

/**
 * Converts a position within a camera to a position within the game.
 */
f32_v2 r_cam_pos_to_game_pos(const r_cam* cam, f32_v2 pos);

/**
 * Determines if a camera can see a position.
 */
bool r_cam_pos_in_fov(const r_cam* cam, f32_v2 pos);

/**
 * Determines if a camera can see a circle.
 */
bool r_cam_circle_in_fov(const r_cam* cam, f32_v2 pos, f32 radius);

/**
 * Determines if a camera can see a axis-aligned square.
 */
bool r_cam_rect_in_fov(const r_cam* cam, f32_v2 pos, f32_v2 size);

#endif
