#ifndef EFFECT_2D_H
#define EFFECT_2D_H

#include <common.h>

/* The type of a 2D effect. */
typedef enum PACKED e_2d_type {
    E_2D_TYPE_EXPLOSION,
    E_2D_TYPE_FLAME,

    // TODO: This smoke simulation could take place on the GPU inside of a
    // shader and just use a pixel buffer and do the math on pixels so no
    // collision detection is required.
    E_2D_TYPE_SMOKE,
} e_2d_type;

/* A 2D effect. */
typedef struct e_2d {
    /* The position of the effect. */
    fu16_v2 pos;

    /* The scale of this effect. */
    fu16_v2 scale;

    /* The color of the effect. */
    rgba8 color;
} e_2d;

/* A 2D explosion effect. */
typedef struct e_2d_explosion {
    /* The parent effect data. */
    e_2d;

    /* The time in ticks since the explosion. */
    u16 time;
} e_2d_explosion;

// TODO: Flame is bound to thrusters and burning objects so there must be some
// kind of way to keep a refrence to the object because that's better than
// updating the effect's data every tick. The best way would be to have
// explosions as one of the only free standing effects then have all the other
// effects bound to the objects in an ECS type of way.

#endif
