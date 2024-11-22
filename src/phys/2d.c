#include <game/rb_2d.h>
#include <phys/2d.h>
#include <phys/grid.h>
#include <math/u32.h>

// TODO: All this has to do to check if a pair was already check is to just
// check if the found index of the same type of rb is lower than the current
// index and if it is the pair was already found.

/* A pair of two rectangle rigid bodies that may be colliding. */
typedef struct pe_rect_rect_pair_2d {
    /* The index of the first rectangle in the global rectangle list. */
    u32 rect_index;

    /* The index of the second rectangle in the global rectangle list. */
    u32 other_rect_index;
} pe_rect_rect_pair_2d;

/*
 * A rectangle rigid body and a circle rigid body pair that may be colliding.
 */
typedef struct pe_rect_circle_pair_2d {
    /* The index of the rectangle in the global rectangle list. */
    u32 rect_index;

    /* The index of the circle in the global circle list. */
    u32 circle_index;
} pe_rect_circle_pair_2d;

/* A pair of two circle rigid bodies that may be colliding. */
typedef struct pe_circle_circle_pair_2d {
    /* The index of the first circle in the global circle list. */
    u32 circle_index;

    /* The index of the other circle in the global circle list. */
    u32 other_circle_index;
} pe_circle_circle_pair_2d;

/* A pair of 2D rigid bodies that may be colliding. */
typedef union pe_pair_2d {
    pe_rect_rect_pair_2d rect_rect;
    pe_rect_circle_pair_2d rect_circle;
    pe_circle_circle_pair_2d circle_circle;
} pe_pair_2d;

/* List of found pairs of 2D rigid bodies that may be colliding. */
static VEC(pe_pair_2d) pe_pairs_2d;

/* The index in p_pairs_2d where rectangle and rectangle pairs end. */
static size_t pe_rect_rect_end = 0;

/* The index in p_pairs_2d where rectangle and circle pairs end. */
static size_t pe_rect_circle_end = 0;

/* The index in p_pairs_2d where circle and circle pairs end. */
static size_t pe_circle_circle_end = 0;

/**
 * Inits the 2D physics engine.
 */
void pe_2d_init()
{
    VEC_INIT(pe_pairs_2d, PE_2D_PAIRS_INIT_LEN);
}

/**
 * Finds pairs of possible collisions between circle rigid bodies.
 */
void pe_find_circle_circle_pairs()
{
#if 0
    if (g_rb_2d_circles.len == 0)
        return;

    /* Adding the pairs to the global pair list. */
    for (size_t i = 0; i < g_rb_2d_circles.len - 1; i++) {
        for (size_t j = i + 1; j < g_rb_2d_circles.len; j++) {
            pe_circle_circle_pair_2d* pair = (void*)VEC_DRY_APPEND(pe_pairs_2d);
            pair->circle_index = i;
            pair->other_circle_index = j;
        }
    }

    /* Setting the index in p_pairs_2d where dual circle pairs end. */
    pe_circle_circle_end = pe_pairs_2d.len;
#endif
    // TODO: Is there a better way to iterate?
    for (size_t i = 0; i < ARRAY_LEN(pe_grid.divisions); i++) {
        u32 obj = pe_grid.divisions[i];
        if (obj == UINT32_MAX || VEC_AT(pe_grid.objs, obj).next == UINT32_MAX)
            continue;

        for (; obj != UINT32_MAX; obj = VEC_AT(pe_grid.objs, obj).next) {
            for (u32 _obj = VEC_AT(pe_grid.objs, obj).next;
            _obj != UINT32_MAX;
            _obj = VEC_AT(pe_grid.objs, _obj).next) {
                pe_circle_circle_pair_2d* pair = (void*)VEC_DRY_APPEND (
                    pe_pairs_2d
                );

                pair->circle_index = VEC_AT(pe_grid.objs, obj).rb & u32_mask_bits(31);
                pair->other_circle_index = VEC_AT(pe_grid.objs, _obj).rb & u32_mask_bits(31);
            }
        }
    }

    /* Setting the index in p_pairs_2d where dual circle pairs end. */
    pe_circle_circle_end = pe_pairs_2d.len;
}

/**
 * Finds all of the 2D rigid body pairs of possible collisions.
 */
void pe_find_pairs()
{
    pe_find_circle_circle_pairs();
}

/**
 * Solves the possible collision of two 2D circle rigid bodies.
 */
void pe_solve_circle_circle(const pe_circle_circle_pair_2d* pair)
{
    o_rb_2d_circle* c0 = &VEC_AT(g_rb_2d_circles, pair->circle_index);
    o_rb_2d_circle* c1 = &VEC_AT(g_rb_2d_circles, pair->other_circle_index);

    /* The combined squared radi of the circles. */
    const f32 radius_sqrd = powf (
        o_rb_2d_circle_radius(c0) + o_rb_2d_circle_radius(c1),
        2
    );

    // TODO: This doesn't account for what if the object's are in the same
    // position maybe that could be done in the broad phrase.
    /* The normal of the collision. */
    f32_v2 normal = f32_v2_sub(c1->obj.pos, c0->obj.pos);

    /* The squared distance between the circles. */
    const f32 dist_sqrd = f32_v2_mag_sqrd(normal);

    /* If the circles are already colliding without scaling. */
    if (radius_sqrd > dist_sqrd)
        goto solve;

    /* If there was no collision. */
    return;

    /* Solves the collision. */
    solve:

    // TODO: Make sure this becomes a inv sqrt.
    normal = f32_v2_mul(normal, f32_v2_splat(1.0f / sqrtf(dist_sqrd)));

    /* The point of the collision. */
    const f32_v2 pos = f32_v2_add (
        c0->obj.pos,
        f32_v2_mul(normal, f32_v2_splat(o_rb_2d_circle_radius(c0)))
    );

    const f32_v2 p0 = f32_v2_sub(pos, c0->obj.pos);
    const f32_v2 p1 = f32_v2_sub(pos, c1->obj.pos);

    f32_v2 vel = f32_v2_add (
        c1->vel,
        (f32_v2) {
            .x = p1.y * -c1->ang_vel,
            .y = p1.x * c1->ang_vel,
        }
    );

    vel = f32_v2_sub(vel, f32_v2_sub (
        c0->vel,
        (f32_v2) {
            .x = p0.y * -c1->ang_vel,
            .y = p0.x * c1->ang_vel,
        }
    ));

    const f32 vel_along_norm = (vel.x * normal.x) + (vel.y * normal.y);

    /* Don't do anything if the objects are seperating. */
    if (vel_along_norm > 0)
        return;

    const pe_mat* mat0 = o_rb_2d_mat((const o_rb_2d*)c0);
    const pe_mat* mat1 = o_rb_2d_mat((const o_rb_2d*)c1);

    const f32 restitution = fmin(mat0->restitution, mat1->restitution);

    const f32 p0_cross = (p0.x * normal.y) - (p0.y * normal.x);
    const f32 p1_cross = (p1.x * normal.y) - (p1.y * normal.x);

    const f32 inv_mass = (f32) (
        (u32)c0->inv_mass + (u32)c1->inv_mass
    ) / (1 << 11);
    
    f32 force = -(1 + restitution) * vel_along_norm;
    force /= inv_mass + (p0_cross * p0_cross) * mat0->inv_inertia
        + (p1_cross * p1_cross) * mat1->inv_inertia;

    const f32_v2 force_vec = f32_v2_mul(normal, f32_v2_splat(force));

    o_rb_2d_apply_force(c0, mat0->inv_inertia, f32_v2_neg(force_vec), p0);
    o_rb_2d_apply_force(c1, mat1->inv_inertia, force_vec, p1);
}

/**
 * Solve all the 2D circle rigid body pairs.
 */
void pe_solve_circle_circle_pairs()
{
    for (size_t i = pe_rect_rect_end; i < pe_circle_circle_end; i++)
        pe_solve_circle_circle(&VEC_AT(pe_pairs_2d, i).circle_circle);
}

/**
 * Solves all of the 2D rigid body pairs of possible collisions.
 */
void pe_solve()
{
    pe_solve_circle_circle_pairs();

    // TODO: This should use some smart algorithm to determine if this should
    // shrink of not.
    pe_pairs_2d.len = 0;
    //VEC_SHRINK(pe_pairs_2d);
}

/**
 * Ticks the 2D physics engine.
 */
void pe_2d_tick()
{
    pe_find_pairs();
    pe_solve();
}
