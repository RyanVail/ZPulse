#ifndef PHYS_ROPE_H
#define PHYS_ROPE_H

#include <game/rb_2d.h>

/**
 * A rope constraint connection two rigid bodies.
 */
typedef struct pe_rope {
    /* The bodies attached. */
    g_rb_2d_id[2] bodies;

    /*
     * The length of the rope. The length threshold where greater at distances
     * force is applied to attract the bodies.
     */
    f32 length;

    /*
     * The strength of the attraction force between the two bodies when the
     * length of the rope surpasses the defined length.
     */
    f32 restituion;

    /* The force required to break the rope. */
    f32 break_force;
} pe_rope;

#endif
