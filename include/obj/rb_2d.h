#ifndef OBJ_RB_2D_H
#define OBJ_RB_2D_H

#include <obj/2d.h>
#include <math/vectors.h>

/* The type of a 2D rigid body. */
typedef enum PACKED o_rb_2d_type {
    O_RB_2D_CIRCLE,
    O_RB_2D_RECT,
    O_RB_2D_AABB,
} o_rb_2d_type;

/* A 2D object's rigid body. */
typedef struct o_rb_2d {
    /* The parent object. */
    o_2d obj;

    /* The velocity of this rigid body. */
    f32_v2 vel;

    /* The inverse mass of this rigid body. */
    f32 inv_mass;
} o_rb_2d;

/* A 2D object's rigid body with a circle based collider. */
typedef struct o_rb_2d_circle {
    /* The parent rigid body. */
    o_rb_2d rb;

    /* The radius of the circle. */
    f32 radius;
} o_rb_2d_circle;

/* A 2D object's rigid body with a rectangle based collider. */
typedef struct o_rb_2d_rect {
    /* The parent rigid body. */
    o_rb_2d rb;

    /* The size of the rectangle. */
    fu16_v2 size;
} o_rb_2d_rect;

/* A 2D object's rigid body with an aabb based collider. */
typedef struct o_rb_2d_aabb {
    /* The parent rigid body. */
    o_rb_2d rb;

    /* The minimum of the aabb. */
    fu16_v2 min;
    
    /* The maximum of the aabb. */
    fu16_v2 max;
} o_rb_2d_aabb;

#endif
