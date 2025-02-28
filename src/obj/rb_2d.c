#include <obj/rb_2d.h>
#include <phys/grid.h>

/**
 * Gets the physics material of a 2D rigid body.
 */
const pe_mat* o_rb_2d_mat(const o_rb_2d* rb)
{
    return pe_get_mat(rb->mat);
}

/**
 * Gets the radius of a 2D circle rigid body.
 */
f32 o_rb_2d_circle_radius(const o_rb_2d* rb)
{
    return o_2d_size_x(&rb->obj);
}

/**
 * Ticks a 2D rigid body.
 */
void o_rb_2d_tick(o_rb_2d* rb)
{
    const pe_mat* mat = o_rb_2d_mat(rb);

    /* Ticking the angular vel of this rigid body. */
    if (rb->ang_vel != 0.0f) {
        rb->obj.rot += (fu16)(rb->ang_vel * 65536.0f);
        rb->ang_vel *= 1.0f - mat->air_res;
    }

    /* Ticking the vel of this rigid body. */
    if (!f32_v2_is_zero(rb->vel)) {
        const f32_v2 old_pos = rb->obj.pos;
        const f32 size = o_rb_2d_circle_radius(rb); // TODO: Should be generic.

        rb->obj.pos = f32_v2_add(rb->obj.pos, rb->vel);
        rb->vel = f32_v2_scale(rb->vel, 1.0f - mat->air_res);

        pe_grid_rb_2d_move (
            g_rb_2d_get_id(rb),
            old_pos,
            size,
            rb->obj.pos,
            size
        );
    }
}

/**
 * Ticks a rectangle 2D rigid body.
 */
void o_rb_2d_rect_tick(o_rb_2d_rect* rect)
{
    o_rb_2d_tick((o_rb_2d*)rect);
}

/**
 * Ticks a circle 2D rigid body.
 */
void o_rb_2d_circle_tick(o_rb_2d_circle* circle)
{
    o_rb_2d_tick((o_rb_2d*)circle);
}

/**
 * Applies an impact force to a 2D rigid body.
 */
void o_rb_2d_apply_impact (
    o_rb_2d* rb,
    f32 inv_inertia,
    f32_v2 force_vec,
    f32_v2 contact_vec )
{
    if (rb->inv_mass == 0)
        return;

    o_rb_2d_apply_force(rb, force_vec);

    rb->ang_vel += inv_inertia * (
        (contact_vec.x * force_vec.y) + (contact_vec.y * force_vec.x)
    );
}

/**
 * Applies a force to a 2D rigid body.
 */
void o_rb_2d_apply_force(o_rb_2d* rb, f32_v2 force_vec)
{
    if (rb->inv_mass == 0)
        return;

    rb->vel = f32_v2_add (
        rb->vel,
        f32_v2_scale(force_vec, o_rb_2d_inv_mass(rb))
    );
}

/**
 * Gets the inverse mass of a 2D rigid body.
 */
f32 o_rb_2d_inv_mass(const o_rb_2d* rb)
{
    return (f32)rb->inv_mass / (1 << 11);
}
