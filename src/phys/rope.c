#include <game/pe_rope.h>
#include <math/f32_v2.h>
#include <math/f32.h>

/**
 * Solves a rope constraint.
 */
static inline void pe_solve_rope(pe_rope* rope)
{
    o_rb_2d* r0 = g_rb_2d_get(rope->bodies[0]);
    o_rb_2d* r1 = g_rb_2d_get(rope->bodies[1]);

    const f32_v2 dif = f32_v2_sub(r0->obj.pos, r1->obj.pos);

    const f32 dist_sqrd = f32_v2_mag_sqrd(dif);
    const f32 rope_len_sqrd = f32_sqr(rope->length);
    if (dist_sqrd <= rope_len_sqrd)
        return;

    f32 force = f32_sqrt(dist_sqrd / rope_len_sqrd) * rope->restitution;

    const f32 force_cap = rope->restitution * PE_ROPE_MAX_FORCE_FACTOR;
    force = f32_clamp(force, -force_cap, force_cap);

    force /= o_rb_2d_inv_mass(r0) + o_rb_2d_inv_mass(r1);

    // TODO: Remove rope.
    //if (force >= rope->break_force)

    const f32_v2 force_vec = f32_v2_mul(dif, f32_v2_splat(force));

    o_rb_2d_apply_force(r0, f32_v2_neg(force_vec));
    o_rb_2d_apply_force(r1, force_vec);
}

void pe_rope_tick()
{
    for VEC_ITER(g_pe_ropes, rope)
        pe_solve_rope(rope);
}
