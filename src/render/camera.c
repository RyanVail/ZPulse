#include <render/camera.h>
#include <math/f32.h>

f32_v2 r_cam_inv_zoom(const r_cam* cam)
{
    return (const f32_v2) {
        1.0f / cam->zoom.x,
        1.0f / cam->zoom.y
    };
}

f32_v2 r_game_pos_to_cam_pos(const r_cam* cam, f32_v2 pos)
{
    return f32_v2_mul(cam->zoom, f32_v2_sub(pos, cam->pos));
}

f32_v2 r_cam_pos_to_game_pos(const r_cam* cam, f32_v2 pos)
{
    return f32_v2_fmadd(pos, r_cam_inv_zoom(cam), cam->pos);
}

bool r_cam_pos_in_fov(const r_cam* cam, f32_v2 pos)
{
    const f32_v2 cam_pos = r_game_pos_to_cam_pos(cam, pos);
    return f32_abs(cam_pos.x) <= 1.0 && f32_abs(cam_pos.y) <= 1.0;
}

bool r_cam_circle_in_fov(const r_cam* cam, f32_v2 pos, f32 radius)
{
    /* Approximate with a square. */
    return r_cam_rect_in_fov(cam, pos, f32_v2_splat(radius * 2.0f));
}

bool r_cam_rect_in_fov(const r_cam* cam, f32_v2 pos, f32_v2 size)
{
    size = f32_v2_scale(size, 0.5f);

    const f32_v2 cam_pos = r_game_pos_to_cam_pos(cam, pos);
    const f32_v2 bound = f32_v2_add(f32_v2_splat(1.0f), size);
    return f32_abs(cam_pos.x) <= bound.x && f32_abs(cam_pos.y) <= bound.y;
}
