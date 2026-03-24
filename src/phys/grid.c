#include <phys/grid.h>
#include <math/u32.h>
#include <math/f32.h>
#include <assert.h>

/* The global grid. */
typeof(pe_grid) pe_grid;

static pe_grid_flat_ids flat_ids_vec;

/**
 * Gets the offset of a quad from it's group parent.
 */
static f32_v2 pe_grid_quad_offset(u32 quad_index, f32 size)
{
    f32_v2 offset;

    if ((quad_index & PE_GRID_VERT_MASK) == PE_GRID_TOP_QUAD) {
        offset.y = size;
    } else {
        offset.y = -size;
    }

    if ((quad_index & PE_GRID_HORZ_MASK) == PE_GRID_RIGHT_QUAD) {
        offset.x = size;
    } else {
        offset.x = -size;
    }

    return offset;
}

/**
 * Frees a set of bodies from the physics grid.
 *
 * @warning This will not free the sub bodies of this set.
 */
static void pe_grid_bodies_free(u32 index, pe_grid_bodies* bodies)
{
    bodies->next_group = pe_grid.first_free_bodies;
    pe_grid.first_free_bodies = index;
}

/**
 * Frees a group from the physics grid.
 *
 * @warning This will not free the sub groups of this group or the linked
 * bodies in this group.
 */
static void pe_grid_group_free(u32 index, pe_grid_group* group)
{
    group->groups[0] = pe_grid.first_free_group;
    pe_grid.first_free_group = index;
}

/**
 * Attempts to free a set of bodies in the physics grid.
 *
 * @param bodies A pointer to the index of the set of bodies. If the value at
 * the pointer is `UINT32_MAX` this will return true.
 * @return True if freed, false otherwise.
 */
static bool pe_grid_bodies_try_free(u32* bodies)
{
    while (true) {
        if (*bodies == UINT32_MAX)
            return true;

        pe_grid_bodies* data = &VEC_AT(pe_grid.bodies, *bodies);
        for (size_t i = 0; i < PE_GRID_GROUP_RB_LEN; i++) {
            if (data->rbs[i] != G_RB_2D_ID_NULL) {
                return false;
            }
        }

        /* Removing the bodies set from the linked list. */
        const u32 old_index = *bodies;
        *bodies = data->next_group;

        pe_grid_bodies_free(old_index, data);
    }

    return true;
}

/**
 * Recursively tries to free group in the physics grid. Only frees the group if
 * it doesn't have any bodies in itself or its children.
 *
 * @return True if freed, false otherwise.
 */
static bool pe_grid_group_try_free(u32 group, u32 depth)
{
    bool can_free = true;
    pe_grid_group* data = &VEC_AT(pe_grid.groups, group);

    /* This group can't be freed if it contains bodies. */
    for (size_t i = 0; i < PE_GRID_GROUP_RB_LEN; i++) {
        if (data->bodies.rbs[i] != G_RB_2D_ID_NULL) {
            can_free = false;
            break;
        }
    }

    /* Trying to free the sub bodies of the bodies set. */
    if (!pe_grid_bodies_try_free(&data->bodies.next_group))
        can_free = false;

    /* Trying to free the sub groups. */
    for (size_t i = 0; i < 4; i++) {
        u32* index = &data->groups[i];

        if (*index == UINT32_MAX)
            continue;

        if (depth == PE_GRID_GROUP_MAX_DEPTH) {
            if (pe_grid_bodies_try_free(index)) {
                *index = UINT32_MAX;
            } else {
                can_free = false;
            }
        } else {
            if (pe_grid_group_try_free(*index, depth + 1)) {
                *index = UINT32_MAX;
            } else {
                can_free = false;
            }
        }
    }

    if (can_free)
        pe_grid_group_free(group, data);

    return can_free;
}

#if DEBUG
#include <render/render.h>

// TODO: This should cull the groups that aren't in the camera's FOV.
static void pe_debug_draw_grid_group(
    const r_cam* cam,
    u32 group,
    u32 depth,
    f32_v2 pos,
    f32 size )
{
    const f32 inv_scale = cam->zoom.y;
    const f32 scale = 1.0f / inv_scale;

    const f32 half = size * 0.5f;
    const f32 scaled_half = half * scale;

    const rgba8 color = {
        ((depth * 2910) & 255) / 2,
        ((depth * 3943) & 255) / 2,
        ((depth * 1359) & 255) / 2,
        0
    };

    r_line (
        cam,
        (const f32_v2) { pos.x - scaled_half, pos.y + scaled_half },
        (const f32_v2) { pos.x - scaled_half, pos.y - scaled_half },
        color
    );

    r_line (
        cam,
        (const f32_v2) { pos.x - scaled_half, pos.y - scaled_half },
        (const f32_v2) { pos.x + scaled_half, pos.y - scaled_half },
        color
    );

    r_line (
        cam,
        (const f32_v2) { pos.x + scaled_half, pos.y + scaled_half },
        (const f32_v2) { pos.x + scaled_half, pos.y - scaled_half },
        color
    );

    r_line (
        cam,
        (const f32_v2) { pos.x + scaled_half, pos.y + scaled_half },
        (const f32_v2) { pos.x - scaled_half, pos.y + scaled_half },
        color
    );

    if (depth == PE_GRID_GROUP_MAX_DEPTH)
        return;

    /* Drawing the subgroups. */
    const pe_grid_group data = VEC_AT(pe_grid.groups, group);
    for (u32 i = 0; i < 4; i++) {
        if (data.groups[i] != UINT32_MAX) {
            pe_debug_draw_grid_group (
                cam,
                data.groups[i],
                depth + 1,
                f32_v2_add(pos, pe_grid_quad_offset(i, half)),
                half
            );
        }
    }
}

void pe_debug_draw_grid(const r_cam* cam)
{
    pe_debug_draw_grid_group (
        cam,
        0,
        0,
        f32_v2_zero(),
        (f32)PE_GRID_SIZE
    );
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
    if (depth == PE_GRID_GROUP_MAX_DEPTH)
        return pe_grid_new_bodies(&groups[quad]);

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
    f32 half = PE_GRID_SIZE * 0.5f;

    u32 group = 0;
    for (u32 depth = 0; depth <= PE_GRID_GROUP_MAX_DEPTH; depth++) {
        /* The index of the quad the origin of this rigid body is in. */
        u32 quad = 0;

        if (pos.x > mid.x) {
            mid.x += half;
            quad |= PE_GRID_RIGHT_QUAD;
        } else {
            mid.x -= half;
            quad |= PE_GRID_LEFT_QUAD;
        }

        if (pos.y > mid.y) {
            mid.y += half;
            quad |= PE_GRID_TOP_QUAD;
        } else {
            mid.y -= half;
            quad |= PE_GRID_BOT_QUAD;
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

void pe_grid_clean()
{
    pe_grid_group_try_free(0, 0);
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
    pe_grid.first_free_group = UINT32_MAX;
    pe_grid.first_free_bodies = UINT32_MAX;

    u32 unused;
    pe_grid_new_group(&unused);
}
