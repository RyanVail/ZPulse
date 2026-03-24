#ifndef PHYS_ROPE_H
#define PHYS_ROPE_H

#include <phys/rope.h>
#include <game/rb_2d.h>

/**
 * A rope constraint connecting two rigid bodies.
 */
typedef struct pe_rope {
    /* The bodies attached. */
    g_rb_2d_id bodies[2];

    /*
     * The length of the rope. The length threshold where greater at distances
     * force is applied to attract the bodies. If the length is zero this rope
     * reacts like a spring, both pulling and attracting till the bodies are
     * the supplied length away from eachother.
     */
    f32 length;

    /*
     * The strength of the attraction force between the two bodies when the
     * length of the rope surpasses the defined length.
     */
    f32 strength;

    /* The force required to break the rope. */
    f32 break_force;
} pe_rope;

/**
 * Checks if a rope constraint should act like a spring.
 */
bool pe_rope_is_spring(const pe_rope* rope);

/**
 * Solves a rope constraint.
 */
bool pe_rope_solve(const pe_rope* rope);

/**
 * Solves all the rope constraints within the 2D physics engine.
 */
void pe_rope_tick();

#endif
