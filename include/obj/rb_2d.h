#ifndef OBJ_RB_2D_H
#define OBJ_RB_2D_H

#include <obj/2d.h>
#include <math/vectors.h>
#include <phys/mat.h>
#include <assert.h>

// TODO: There should be asserts somewhere to make sure these are all 32 bytes.
// TODO: If this isn't required remove it.
/* The type of a 2D rigid body. */
typedef enum PACKED o_rb_2d_type {
    O_RB_2D_RECT = 0,
    O_RB_2D_CIRCLE = 1,
} o_rb_2d_type;

/* A 2D object's rigid body. */
typedef struct o_rb_2d {
    /* The parent object. */
    o_2d obj;

    /* The velocity of this rigid body. */
    f32_v2 vel;

    /* The angular velocity of this rigid body. */
    f32 ang_vel;

    /* The inverse mass of this rigid body. */
    fu16 inv_mass;

    /* The id of the physics material of this rigid body. */
    pe_mat_id mat;
} o_rb_2d;

/* Perf oriented code relies on this being 32 bytes. */
static_assert(sizeof(o_rb_2d) == 32, "The size of o_rb_2d isn't 32 bytes.");

/* A 2D object's rigid body with a rectangle collider. */
typedef o_rb_2d o_rb_2d_rect;

/* A 2D object's rigid body with a circle collider. */
typedef o_rb_2d o_rb_2d_circle;

/**
 * Gets the physics material of a 2D rigid body.
 */
const pe_mat* o_rb_2d_mat(const o_rb_2d* rb);

/**
 * Gets the radius of a 2D circle rigid body.
 */
f32 o_rb_2d_circle_radius(const o_rb_2d* rb);

/**
 * Ticks a 2D rigid body.
 */
void o_rb_2d_tick(o_rb_2d* rb);

/**
 * Ticks a rectangle 2D rigid body.
 */
void o_rb_2d_rect_tick(o_rb_2d_rect* rect);

/**
 * Ticks a circle 2D rigid body.
 */
void o_rb_2d_circle_tick(o_rb_2d_circle* circle);

/**
 * Applies a force to a 2D rigid body.
 */
void o_rb_2d_apply_force (
    o_rb_2d* rb,
    f32 inv_inertia,
    f32_v2 force_vec,
    f32_v2 contact_vec );

/**
 * Gets the inverse mass of a 2D rigid body.
 */
f32 o_rb_2d_inv_mass(const o_rb_2d* rb);

#endif
