#include <obj/2d.h>

r_tex o_2d_tex(const o_2d* obj)
{
    return o_2d_sprite(obj)->tex;
}

const o_sprite_2d* o_2d_sprite(const o_2d* obj)
{
    return o_sprite(obj->sprite);
}

rgba8 o_2d_color(const o_2d* obj)
{
    return o_2d_sprite(obj)->color;
}

m_angle o_2d_ang(const o_2d* obj)
{
    return ((m_angle)obj->rot / 65535.0f) * (m_angle)(2 * M_PI);
}

f32 o_2d_rot_sin(const o_2d* obj)
{
    // TODO: Implement a better algorithm for this.
    return (f32)sin(((f32)obj->rot / 65535.0f) * (2 * (f32)M_PI));
}

f32 o_2d_rot_cos(const o_2d* obj)
{
    // TODO: Implement a better algorithm for this.
    return (f32)cos(((f32)obj->rot / 65535.0f) * (2 * (f32)M_PI));
}

f32 o_2d_size_x(const o_2d* obj)
{
    return (f32)(obj->scale.x << 7) / 65536;
}

f32 o_2d_size_y(const o_2d* obj)
{
    return (f32)(obj->scale.y << 7) / 65536;
}

f32_v2 o_2d_size_f32_v2(const o_2d* obj)
{
    return (f32_v2) {
        .x = o_2d_size_x(obj),
        .y = o_2d_size_y(obj),
    };
}
