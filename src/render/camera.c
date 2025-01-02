#include <render/camera.h>

/**
 * Converts a position within a camera to a position within the game.
 */
f32_v2 r_cam_pos_to_game_pos(const r_cam* cam, const f32_v2 pos)
{
    return f32_v2_add (
        cam->pos,
        f32_v2_mul (
            pos,
            (const f32_v2) {
                1.0f / cam->zoom.x,
                1.0f / cam->zoom.y
            }
        )
    );
}
