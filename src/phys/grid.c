#include <phys/grid.h>
#include <math/u32.h>
#include <math/f32.h>
#include <assert.h>

/* The global grid. */
typeof(pe_grid) pe_grid;

static pe_grid_flat_ids flat_ids_vec;

#if DEBUG
#include <render/render.h>
/**
 * Draws the physics grid for debugging.
 */
void pe_debug_draw_grid(const r_cam* cam)
{
#if 0
    /* Used to tell how many cells should be drawn. */
    const f32 inv_scale = cam->zoom.y;
    const f32 scale = 1.0f / inv_scale;

    /* The divisions per grid cell, each grid cell is one unit. */
    const i32 max_div = (1 << PE_GRID_DIVISIONS) / PE_GRID_SIZE;

    /* The division the camera's position is in. */
    const i32 int_x_div = (i32)(cam->pos.x * (f32)max_div);
    const i32 int_y_div = (i32)(cam->pos.y * (f32)max_div);

    /* The distance apart each line in the grid is. */
    const f32 inc = (f32)PE_GRID_SIZE / (1 << PE_GRID_DIVISIONS);

    const f32 base_x = int_x_div * inc;
    const f32 base_y = int_y_div * inc;

    for (i32 y = -max_div * scale; y <= max_div * scale; y++) {
        r_line (
            cam,
            (const f32_v2) { base_x - scale, base_y + (inc * y) },
            (const f32_v2) { base_x + scale, base_y + (inc * y) },
            (const rgba8) {
                (((int_y_div + y) * 2910) & 255) / 4,
                (((int_y_div + y) * 3943) & 255) / 4,
                (((int_y_div + y) * 1359) & 255) / 4,
                0
            }
        );
    }

    for (i32 x = -max_div * scale; x <= max_div * scale; x++) {
        r_line (
            cam,
            (const f32_v2) { base_x + (inc * x), base_y - scale },
            (const f32_v2) { base_x + (inc * x), base_y + scale },
            (const rgba8) {
                (((int_x_div + x) * 2910) & 255) / 4,
                (((int_x_div + x) * 3943) & 255) / 4,
                (((int_x_div + x) * 1359) & 255) / 4,
                0
            }
        );
    }
#endif
}
#endif


/**
 * Adds a new group to the physics grid.
 *
 * @param index_dest A pointer to the location where the index of the group
 * should be written before any pointers to groups become invalid.
 * @return The index of the group.
 *
 * @warning Any pointers to groups become invalid after this function is
 * called.
 */
u32 pe_grid_new_group(u32* index_dest)
{
    const u32 index = pe_grid.groups.len;
    *index_dest = index;

    pe_grid_group* group = VEC_DRY_APPEND(pe_grid.groups);
    // TODO: This is bad.
    memset(group, 255, sizeof(*group));
    return index;
}

/**
 * Adds a new group of bodies to the physics grid.
 *
 * @param index_dest A pointer to the location where the index of the bodies
 * should be written before any pointers to bodies become invalid.
 * @return The group of bodies index.
 *
 * @warning Any pointers to bodies become invalid after this function is
 * called.
 */
u32 pe_grid_new_bodies(u32* index_dest)
{
    const u32 index = pe_grid.bodies.len;
    *index_dest = index;

    pe_grid_bodies* bodies = VEC_DRY_APPEND(pe_grid.bodies);
    // TODO: This is bad.
    memset(bodies, 255, sizeof(*bodies));
    return index;
}

/**
 * Gets the sub group of a group at a specified quad index. If the sub group
 * is null this will init it.
 */
static u32 pe_grid_get_subgroup(u32 group, u32 depth, u32 quad)
{
    u32* groups = VEC_AT(pe_grid.groups, group).groups;
    if (groups[quad] != UINT32_MAX)
        return groups[quad];

    /* Initializing a new group. */
    if (depth == PE_GRID_GROUP_MAX_DEPTH) {
        return pe_grid_new_bodies(&groups[quad]);
    }

    return pe_grid_new_group(&groups[quad]);
}

/**
 * Gets the grid bodies group for a position and size.
 *
 * @warning All pointers to bodies within the physics grid become invalid after
 * calling this function.
 */
pe_grid_bodies* pe_grid_get_bodies(const f32_v2 pos, f32 size)
{
    f32_v2 mid = f32_v2_splat(0.0f);
    f32 half = PE_GRID_SIZE / 4.0f;

    u32 group = 0;
    for (u32 depth = 0; depth <= PE_GRID_GROUP_MAX_DEPTH; depth++) {
        /* The index of the quad the origin of this rigid body is in. */
        u32 quad = 0;

        if (pos.x > mid.x) {
            mid.x += half;
        } else {
            mid.x -= half;
            quad |= 1;
        }

        if (pos.y > mid.y) {
            mid.y += half;
        } else {
            mid.y -= half;
            quad |= 2;
        }

        /* Checking if the body fits within the quad. */
        if (f32_abs(mid.x - pos.x) < size || f32_abs(mid.y - pos.y) < size)
            break;

        group = pe_grid_get_subgroup(group, depth, quad);
        if (depth == PE_GRID_GROUP_MAX_DEPTH)
            return &VEC_AT(pe_grid.bodies, group);

        half *= 0.5f;
    }

    return &VEC_AT(pe_grid.groups, group).bodies;
}

static void pe_grid_rb_2d_add_to_bodies(pe_grid_bodies* bodies, g_rb_2d_id id)
{
    while (true) {
        for (size_t i = 0; i < ARRAY_LEN(bodies->rbs); i++) {
            if (bodies->rbs[i] == G_RB_2D_ID_NULL) {
                bodies->rbs[i] = id;
                return;
            }
        }

        if (bodies->next_group == UINT32_MAX) {
            const u32 next_group = pe_grid_new_bodies(&bodies->next_group);
            VEC_AT(pe_grid.bodies, next_group).rbs[0] = id;
            return;
        }

        bodies = &VEC_AT(pe_grid.bodies, bodies->next_group);
    }
}

static void pe_grid_rb_2d_remove_from_bodies (
    pe_grid_bodies* bodies,
    g_rb_2d_id id )
{
    while (true) {
        for (size_t i = 0; i < ARRAY_LEN(bodies->rbs); i++) {
            // TODO: This should be trying to free the group too.
            if (bodies->rbs[i] == id) {
                bodies->rbs[i] = G_RB_2D_ID_NULL;
                return;
            }
        }

        DEBUG_ASSERT (bodies->next_group != UINT32_MAX,
            "Couldn't find object id when removing object from physics grid"
        )

        bodies = &VEC_AT(pe_grid.bodies, bodies->next_group);
    }
}

/**
 * Adds a 2D rigid body to the physics grid.
 */
void pe_grid_rb_2d_add(g_rb_2d_id id, const f32_v2 pos, f32 size)
{
    pe_grid_rb_2d_add_to_bodies(pe_grid_get_bodies(pos, size), id);
}

/**
 * If required, moves a 2D rigid body in the physics grid after a size and or a
 * position change.
 */
void pe_grid_rb_2d_move (
    g_rb_2d_id id,
    const f32_v2 old_pos,
    f32 old_size,
    const f32_v2 new_pos,
    f32 new_size )
{
    /*
     * New bodies must be gotten before old bodies, because the new bodies may
     * resize the bodies vec making the old bodies ptr invalid.
     */
    pe_grid_bodies* new_bodies = pe_grid_get_bodies(new_pos, new_size);
    pe_grid_bodies* old_bodies = pe_grid_get_bodies(old_pos, old_size);

    /* Moving the object in the physics grid if its group changed. */
    if (old_bodies != new_bodies) {
        pe_grid_rb_2d_remove_from_bodies(old_bodies, id);
        pe_grid_rb_2d_add_to_bodies(new_bodies, id);
    }
}

/**
 * Flattens a group of bodies within the physics quad tree.
 */
static void pe_grid_flatten_bodies(const pe_grid_bodies* bodies)
{
    while (true) {
        /* Adding all the rbs in the group to the flat id vec. */
        for (size_t i = 0; i < ARRAY_LEN(bodies->rbs); i++)
            if (bodies->rbs[i] != G_RB_2D_ID_NULL)
                VEC_APPEND(flat_ids_vec, &bodies->rbs[i]);

        if (bodies->next_group == UINT32_MAX)
            break;

        /* Adding the rbs in the next bodies group. */
        bodies = &VEC_AT(pe_grid.bodies, bodies->next_group);
    }
}

/**
 * Flattens a group within the physics quad tree.
 */
static void pe_grid_flatten_group(const pe_grid_group* group, u32 depth)
{
    /* Adding the bodies within this group to the vec. */
    pe_grid_flatten_bodies(&group->bodies);

    /* Flattening this group's sub groups. */
    for (size_t i = 0; i < ARRAY_LEN(group->groups); i++) {
        if (group->groups[i] == UINT32_MAX)
            continue;

        *VEC_DRY_APPEND(flat_ids_vec) = PE_GRID_RB_DEPTH_DOWN;

        if (depth == PE_GRID_GROUP_MAX_DEPTH) {
            pe_grid_flatten_bodies(&VEC_AT(pe_grid.bodies, group->groups[i]));
        } else {
            pe_grid_flatten_group (
                &VEC_AT(pe_grid.groups, group->groups[i]),
                depth + 1
            );
        }

        *VEC_DRY_APPEND(flat_ids_vec) = PE_GRID_RB_DEPTH_UP;
    }
}

/**
 * Flattens the physics quad tree into a single vector.
 *
 * @warning The vector returned by this function is valid up until this
 * function is called again.
 */
const pe_grid_flat_ids* pe_grid_flatten()
{
    flat_ids_vec.len = 0;
    pe_grid_flatten_group(&VEC_AT(pe_grid.groups, 0), 0);
    return &flat_ids_vec;
}

/**
 * Inits the physics engine's grid.
 */
void pe_grid_init()
{
    // TODO: Choose a real number.
    VEC_INIT(pe_grid.groups, 4);
    VEC_INIT(pe_grid.bodies, 4);
    VEC_INIT(flat_ids_vec, 128);
    pe_grid.first_empty_group = UINT32_MAX;
    pe_grid.first_empty_bodies = UINT32_MAX;

    u32 unused;
    pe_grid_new_group(&unused);
}
