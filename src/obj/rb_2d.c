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
        const u32 old_grid_divison = pe_grid_division(rb->obj.pos);
        rb->obj.pos = f32_v2_add(rb->obj.pos, rb->vel);
        rb->vel = f32_v2_mul(rb->vel, f32_v2_splat(1.0f - mat->air_res));

        /* Readding this object to the grid if it moved grid divisions. */
        const u32 new_grid_divison = pe_grid_division(rb->obj.pos);
        if (new_grid_divison != old_grid_divison)
            pe_grid_rb_2d_move(rb, old_grid_divison, new_grid_divison);
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

// TODO: Rename the inputs of this function to make more sense and document it
// more.
/**
 * Applies a force to a 2D rigid body.
 */
void o_rb_2d_apply_force (
    o_rb_2d* rb,
    f32 inv_inertia,
    f32_v2 force_vec,
    f32_v2 contact_vec )
{
    if (rb->inv_mass == 0)
        return;

    rb->vel = f32_v2_add (
        rb->vel,
        f32_v2_mul (
            force_vec,
            f32_v2_splat(o_rb_2d_inv_mass(rb))
        )
    );

    rb->ang_vel += inv_inertia * (
        (contact_vec.x * force_vec.y) + (contact_vec.y * force_vec.x)
    );
}

/**
 * Gets the inverse mass of a 2D rigid body.
 */
f32 o_rb_2d_inv_mass(const o_rb_2d* rb)
{
    return (f32)rb->inv_mass / (1 << 11);
}
