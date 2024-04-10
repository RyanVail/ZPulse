#ifndef OBJ_RB_2D_H
#define OBJ_RB_2D_H

#include <obj/2d.h>
#include <math/vectors.h>

/* The type of a 2D rigid body. */
typedef enum PACKED o_rb_2d_type {
    O_RB_2D_CIRCLE,
    O_RB_2D_RECTANGLE,
} o_rb_2d_type;

/* A 2D object's rigid body. */
typedef struct o_rb_2d {
    /* The parent object this rigid body is attached to. */
    o_2d obj;

    /* The velocity of this rigid body. */
    f32_v2 vel;

    /* The inverse mass of this rigid body. */
    f32 inv_mass;

    /* The type of this rigid body. */
    o_rb_2d_type type;
} o_rb_2d;

#endif
