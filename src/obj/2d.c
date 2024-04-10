#include <obj/2d.h>

/**
 * Gets the texture of a 2D object.
 */
r_tex o_2d_tex(const o_2d* obj)
{
    return o_sprite(obj->sprite)->tex;
}

/**
 * Gets the color of a 2D object.
 */
rgba8 o_2d_color(const o_2d* obj)
{
    return rgba8_blend (
        o_sprite(obj->sprite)->color,
        obj->color
    );
}

/**
 * Calculates the sin of a 2D object's rotation.
 */
f32 o_2d_rot_sin(const o_2d* obj)
{
    // TODO: Implement a better algorithm for this.
    return (f32)sin(((f32)obj->rot / (f32)65535) * (2 * (f32)M_PI));
}

/**
 * Calculates the cos of a 2D object's rotation.
 */
f32 o_2d_rot_cos(const o_2d* obj)
{
    // TODO: Implement a better algorithm for this.
    return (f32)cos(((f32)obj->rot / (f32)65535) * (2 * (f32)M_PI));
}

/**
 * Converts a 2D object's size into a f32_v2.
 */
f32_v2 o_2d_size_f32_v2(const o_2d* obj)
{
    // TODO: Find the best solution for this.
    return (f32_v2) {
        .x = (f32)(obj->scale.x << 7) / 65536,
        .y = (f32)(obj->scale.y << 7) / 65536,
    };
}
