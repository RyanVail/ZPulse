#ifndef OBJ_2D_H
#define OBJ_2D_H

#include <math/vectors.h>
#include <utils/color.h>
#include <obj/sprite_2d.h>
#include <math/angle.h>

/* A 2D object. */
typedef struct o_2d {
    // TODO: Positions should actually be u32s.
    /* The position of this object. */
    f32_v2 pos;

    /* The scale of this object. */
    fu16_v2 scale;

    /* The rotation of this object. */
    fu16 rot;

    // TODO: This should be moved out of the object definition so the objects
    // can hold their location in the physics grid.
    /* The id of the sprite of this object. */
    o_sprite_id_2d sprite;
} o_2d;

/**
 * Gets the texture of a 2D object.
 */
r_tex o_2d_tex(const o_2d* obj);

/**
 * Gets the sprite of a 2D object.
 */
const o_sprite_2d* o_2d_sprite(const o_2d* obj);

/**
 * Gets the color of a 2D object.
 */
rgba8 o_2d_color(const o_2d* obj);

/**
 * Calculates the angle in radians of a 2D object.
 */
m_angle o_2d_ang(const o_2d* obj);

/**
 * Calculates the sin of a 2D object's rotation.
 */
f32 o_2d_rot_sin(const o_2d* obj);

/**
 * Calculates the cos of a 2D object's rotation.
 */
f32 o_2d_rot_cos(const o_2d* obj);

/**
 * Converts the x cord of a 2D object's size into a f32.
 */
f32 o_2d_size_x(const o_2d* obj);

/**
 * Converts the y cord of a 2D object's size into a f32.
 */
f32 o_2d_size_y(const o_2d* obj);

/**
 * Converts a 2D object's size into a f32_v2.
 */
f32_v2 o_2d_size_f32_v2(const o_2d* obj);

#endif
