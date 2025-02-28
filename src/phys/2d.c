#include <game/rb_2d.h>
#include <phys/2d.h>
#include <phys/grid.h>
#include <math/u32.h>
#include <math/f32.h>
#include <phys/rope.h>

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

#if 0
/**
 * Finds pairs of possible collisions between circle rigid bodies.
 */
void pe_find_circle_circle_pairs()
{
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
#endif

static void pe_find_pairs_for (
    const pe_grid_flat_ids* ids,
    size_t index,
    const g_rb_2d_id finding_id )
{
    index += 1;
    u32 depth = 0;
    for (size_t i = index; i < ids->len; i++) {
        const g_rb_2d_id id = VEC_AT(*ids, i);

        if (id == PE_GRID_RB_DEPTH_DOWN) {
            depth++;
        } else if (id == PE_GRID_RB_DEPTH_UP) {
            /* This body cannot collide with any more bodies. */
            if (depth == 0)
                return;

            depth--;
        } else {
            pe_circle_circle_pair_2d* pair = (void*)VEC_DRY_APPEND(pe_pairs_2d);
            pair->circle_index = finding_id & u32_mask_bits(31);
            pair->other_circle_index = id & u32_mask_bits(31);
        }
    }
}

/**
 * Finds all of the 2D rigid body pairs of possible collisions.
 */
void pe_find_pairs()
{
    const pe_grid_flat_ids* ids = pe_grid_flatten();
    for (size_t i = 0; i < ids->len; i++) {
        const g_rb_2d_id id = VEC_AT(*ids, i);
        if (id == PE_GRID_RB_DEPTH_DOWN || id == PE_GRID_RB_DEPTH_UP)
            continue;

        pe_find_pairs_for(ids, i, id);
    }

    // TODO: This has to be changed.
    /* Setting the index in p_pairs_2d where dual circle pairs end. */
    pe_circle_circle_end = pe_pairs_2d.len;
}

static void pe_solve_intersection (
    o_rb_2d* r0,
    o_rb_2d* r1,
    f32 combined_inv_mass,
    f32_v2 normal,
    f32 overlap )
{
    // TODO: This calculation could be done before calling this function for
    // perf.
    /* If these bodies aren't intersecting. */
    if (overlap < PE_INTERSECTION_MIN_OVERLAY)
        return;

    const f32 force = overlap * PE_INTERSECTION_FIX_RATIO / combined_inv_mass;

    const f32_v2 force_vec = f32_v2_mul(normal, f32_v2_splat(force));

    const f32_v2 r0_old_pos = r0->obj.pos;
    const f32_v2 r1_old_pos = r1->obj.pos;

    const f32 r0_size = o_rb_2d_circle_radius(r0);
    const f32 r1_size = o_rb_2d_circle_radius(r1);

    r0->obj.pos = f32_v2_sub (
        r0->obj.pos,
        f32_v2_mul(force_vec, f32_v2_splat(o_rb_2d_inv_mass(r0)))
    );

    r1->obj.pos = f32_v2_add (
        r1->obj.pos,
        f32_v2_mul(force_vec, f32_v2_splat(o_rb_2d_inv_mass(r1)))
    );

    pe_grid_rb_2d_move (
        g_rb_2d_get_id(r0),
        r0_old_pos,
        r0_size,
        r0->obj.pos,
        r0_size
    );

    pe_grid_rb_2d_move (
        g_rb_2d_get_id(r1),
        r1_old_pos,
        r1_size,
        r1->obj.pos,
        r1_size
    );
}

/**
 * Solves the possible collision of two 2D circle rigid bodies.
 */
void pe_solve_circle_circle(const pe_circle_circle_pair_2d* pair)
{
    o_rb_2d_circle* c0 = &VEC_AT(g_rb_2d_circles, pair->circle_index);
    o_rb_2d_circle* c1 = &VEC_AT(g_rb_2d_circles, pair->other_circle_index);

    /* The combined squared radi of the circles. */
    const f32 radius_sqrd = f32_sqr (
        o_rb_2d_circle_radius(c0) + o_rb_2d_circle_radius(c1)
    );

    // TODO: This doesn't account for what if the object's are in the same
    // position maybe that could be done in the broad phrase.
    /* The normal of the collision. */
    f32_v2 normal = f32_v2_sub(c1->obj.pos, c0->obj.pos);

    /* The squared distance between the circles. */
    const f32 dist_sqrd = f32_v2_mag_sqrd(normal);

    /* If there was no collision. */
    if (radius_sqrd <= dist_sqrd)
        return;

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

    const f32 inv_mass = (f32) (
        (u32)c0->inv_mass + (u32)c1->inv_mass
    ) / (1 << 11);

    pe_solve_intersection (
        c0,
        c1,
        inv_mass,
        normal,
        1.0f - (f32_sqrt(dist_sqrd / radius_sqrd))
    );

    /* Don't do anything impact processing if the objects are seperating. */
    if (vel_along_norm > 0)
        return;

    const pe_mat* mat0 = o_rb_2d_mat((const o_rb_2d*)c0);
    const pe_mat* mat1 = o_rb_2d_mat((const o_rb_2d*)c1);

    const f32 restitution = fmin(mat0->restitution, mat1->restitution);

    const f32 p0_cross = (p0.x * normal.y) - (p0.y * normal.x);
    const f32 p1_cross = (p1.x * normal.y) - (p1.y * normal.x);

    f32 force = -(1 + restitution) * vel_along_norm;
    force /= inv_mass + (p0_cross * p0_cross) * mat0->inv_inertia
        + (p1_cross * p1_cross) * mat1->inv_inertia;

    const f32_v2 force_vec = f32_v2_mul(normal, f32_v2_splat(force));

    o_rb_2d_apply_impact(c0, mat0->inv_inertia, f32_v2_neg(force_vec), p0);
    o_rb_2d_apply_impact(c1, mat1->inv_inertia, force_vec, p1);
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
    pe_rope_tick();
}
