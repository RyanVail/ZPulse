#include <phys/grid.h>
#include <phys/debug.h>
#include <math/u32.h>
#include <math/f32.h>
#include <assert.h>
#include <render/render.h>

/* The global grid. */
struct {
    VEC(pe_grid_group) groups;
    VEC(pe_grid_bodies) bodies;

    pe_grid_group_id first_free_group;
    pe_grid_bodies_id first_free_bodies;
} pe_grid;

/*
 * A flat version of the physics grid quad tree. *PE_GRID_RB_DEPTH_UP* and
 * *PE_GRID_RB_DEPTH_DOWN* denote changes in depth.
 */
static pe_grid_flat_ids flat_ids_vec;

/**
 * Gets the offset of a quad from it's group parent.
 */
static f32_v2 pe_grid_quad_offset(pe_grid_quad quad, f32 size)
{
    switch (quad)
    {
    case PE_GRID_QUAD_TOP_LEFT:  return (f32_v2) { -size, size };
    case PE_GRID_QUAD_TOP_RIGHT: return (f32_v2) {  size,  size };
    case PE_GRID_QUAD_BOT_LEFT:  return (f32_v2) { -size, -size };
    case PE_GRID_QUAD_BOT_RIGHT: return (f32_v2) {  size, -size };
    default: UNREACHABLE();
    }
}

/**
 * Frees a set of bodies from the physics grid.
 *
 * @warning This will not free the sub bodies of this set.
 */
static void pe_grid_bodies_free(pe_grid_bodies_id id, pe_grid_bodies* bodies)
{
    bodies->next = pe_grid.first_free_bodies;
    pe_grid.first_free_bodies = id;
}

/**
 * Frees a group from the physics grid.
 *
 * @warning This will not free the sub groups of this group or the linked
 * bodies in this group.
 */
static void pe_grid_group_free(pe_grid_group_id id, pe_grid_group* group)
{
    group->quads[0] = pe_grid.first_free_group;
    pe_grid.first_free_group = id;
}

/**
 * Attempts to free a set of bodies in the physics grid.
 *
 * @param bodies A pointer to the index of the set of bodies. If the value at
 * the pointer is null this will return true.
 * @return True if freed, false otherwise.
 */
static bool pe_grid_bodies_try_free(pe_grid_bodies_id* bodies)
{
    while (true) {
        if (*bodies == PE_GRID_BODIES_ID_NULL)
            return true;

        pe_grid_bodies* data = &VEC_AT(pe_grid.bodies, *bodies);
        for (size_t i = 0; i < PE_GRID_GROUP_RB_LEN; i++) {
            if (data->rbs[i] != G_RB_2D_ID_NULL) {
                return false;
            }
        }

        /* Removing the bodies set from the linked list. */
        const pe_grid_bodies_id old_id = *bodies;
        *bodies = data->next;

        pe_grid_bodies_free(old_id, data);
    }

    return true;
}

/**
 * Recursively tries to free group in the physics grid. Only frees the group if
 * it doesn't have any bodies in itself or its children.
 *
 * @return True if freed, false otherwise.
 */
static bool pe_grid_group_try_free(pe_grid_group_id group, u32 depth)
{
    bool can_free = true;
    pe_grid_group* data = &VEC_AT(pe_grid.groups, group);

    /* This group can't be freed if it contains bodies. */
    if (!pe_grid_bodies_try_free(&data->bodies))
        can_free = false;

    /* Trying to free the sub groups. */
    for (size_t i = 0; i < PE_GRID_QUADS; i++) {
        pe_grid_bodies_id* id = &data->quads[i];

        if (*id == PE_GRID_BODIES_ID_NULL)
            continue;

        if (depth == PE_GRID_GROUP_MAX_DEPTH) {
            if (pe_grid_bodies_try_free(id)) {
                *id = PE_GRID_GROUP_ID_NULL;
            } else {
                can_free = false;
            }
        } else {
            if (pe_grid_group_try_free(*id, depth + 1)) {
                *id = PE_GRID_GROUP_ID_NULL;
            } else {
                can_free = false;
            }
        }
    }

    if (can_free)
        pe_grid_group_free(group, data);

    return can_free;
}

// TODO: This should cull the groups that aren't in the camera's FOV.
static void pe_debug_draw_grid_group(
    const r_cam* cam,
    pe_grid_group_id group,
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
        (f32_v2) { pos.x - scaled_half, pos.y + scaled_half },
        (f32_v2) { pos.x - scaled_half, pos.y - scaled_half },
        color
    );

    r_line (
        cam,
        (f32_v2) { pos.x - scaled_half, pos.y - scaled_half },
        (f32_v2) { pos.x + scaled_half, pos.y - scaled_half },
        color
    );

    r_line (
        cam,
        (f32_v2) { pos.x + scaled_half, pos.y + scaled_half },
        (f32_v2) { pos.x + scaled_half, pos.y - scaled_half },
        color
    );

    r_line (
        cam,
        (f32_v2) { pos.x + scaled_half, pos.y + scaled_half },
        (f32_v2) { pos.x - scaled_half, pos.y + scaled_half },
        color
    );

    if (depth == PE_GRID_GROUP_MAX_DEPTH)
        return;

    /* Drawing the subgroups. */
    const pe_grid_group data = VEC_AT(pe_grid.groups, group);
    for (size_t i = 0; i < PE_GRID_QUADS; i++) {
        if (data.quads[i] != PE_GRID_GROUP_ID_NULL) {
            pe_debug_draw_grid_group (
                cam,
                data.quads[i],
                depth + 1,
                f32_v2_add(pos, pe_grid_quad_offset(i, half)),
                half
            );
        }
    }
}

void pe_debug_draw_grid(const r_cam* cam)
{
    if (!DEBUG)
        return;

    pe_debug_draw_grid_group (
        cam,
        0,
        0,
        f32_v2_zero(),
        (f32)PE_GRID_SIZE
    );
}

static pe_grid_group_id pe_grid_new_group(pe_grid_group_id* id_dest)
{
    const pe_grid_group_id id = (pe_grid_group_id)pe_grid.groups.len;
    *id_dest = id;

    pe_grid_group* group = VEC_DRY_APPEND(pe_grid.groups);

    /* Initing the group. */
    group->bodies = PE_GRID_BODIES_ID_NULL;
    for (size_t i = 0; i < PE_GRID_QUADS; i++) {
        group->quads[i] = PE_GRID_GROUP_ID_NULL;
    }

    return id;
}

static pe_grid_bodies_id pe_grid_new_bodies(pe_grid_bodies_id* id_dest)
{
    const pe_grid_bodies_id id = (pe_grid_bodies_id)pe_grid.bodies.len;
    *id_dest = id;

    pe_grid_bodies* bodies = VEC_DRY_APPEND(pe_grid.bodies);

    /* Initing the bodies. */
    bodies->next = PE_GRID_BODIES_ID_NULL;
    for (size_t i = 0; i < PE_GRID_GROUP_RB_LEN; i++) {
        bodies->rbs[i] = G_RB_2D_ID_NULL;
    }

    return id;
}

/**
 * Gets a sub quad of a group. If the quad is null this will init it.
 */
static pe_grid_group_id pe_grid_get_subgroup(
    pe_grid_group_id id,
    u32 depth,
    pe_grid_quad quad )
{
    pe_grid_group* group = &VEC_AT(pe_grid.groups, id);
    if (group->quads[quad] != PE_GRID_GROUP_ID_NULL)
        return group->quads[quad];

    /* Initializing a new group. */
    if (depth == PE_GRID_GROUP_MAX_DEPTH)
        return pe_grid_new_bodies(&group->quads[quad]);

    return pe_grid_new_group(&group->quads[quad]);
}

pe_grid_bodies* pe_grid_get_bodies(f32_v2 pos, f32 size)
{
    f32_v2 mid = f32_v2_splat(0.0f);
    f32 half = PE_GRID_SIZE * 0.5f;

    pe_grid_group_id group = 0;
    for (u32 depth = 0; depth <= PE_GRID_GROUP_MAX_DEPTH; depth++) {
        /* The quad the origin of the rigid body is inside. */
        pe_grid_quad quad = 0;

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

    pe_grid_group* group_ptr = &VEC_AT(pe_grid.groups, group);

    pe_grid_bodies_id bodies = group_ptr->bodies;
    if (bodies == PE_GRID_BODIES_ID_NULL) {
        bodies = pe_grid_new_bodies(&group_ptr->bodies);
    }

    return &VEC_AT(pe_grid.bodies, group_ptr->bodies);
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

        if (bodies->next == PE_GRID_BODIES_ID_NULL) {
            const pe_grid_group_id next = pe_grid_new_bodies(&bodies->next);
            VEC_AT(pe_grid.bodies, next).rbs[0] = id;
            return;
        }

        bodies = &VEC_AT(pe_grid.bodies, bodies->next);
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

        DEBUG_ASSERT (bodies->next != PE_GRID_BODIES_ID_NULL,
            "Couldn't find object id when removing object from physics grid"
        );

        bodies = &VEC_AT(pe_grid.bodies, bodies->next);
    }
}

void pe_grid_rb_2d_add(g_rb_2d_id id, f32_v2 pos, f32 size)
{
    pe_grid_rb_2d_add_to_bodies(pe_grid_get_bodies(pos, size), id);
}

void pe_grid_rb_2d_move (
    g_rb_2d_id id,
    f32_v2 old_pos,
    f32 old_size,
    f32_v2 new_pos,
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

        if (bodies->next == PE_GRID_BODIES_ID_NULL)
            break;

        /* Adding the rbs in the next bodies group. */
        bodies = &VEC_AT(pe_grid.bodies, bodies->next);
    }
}

/**
 * Flattens a group within the physics quad tree.
 */
static void pe_grid_flatten_group(const pe_grid_group* group, u32 depth)
{
    /* Adding the bodies within this group to the vec. */
    if (group->bodies != PE_GRID_BODIES_ID_NULL) {
        pe_grid_flatten_bodies(&VEC_AT(pe_grid.bodies, group->bodies));
    }

    /* Flattening this group's sub groups. */
    for (size_t i = 0; i < PE_GRID_QUADS; i++) {
        if (group->quads[i] == PE_GRID_GROUP_ID_NULL)
            continue;

        *VEC_DRY_APPEND(flat_ids_vec) = PE_GRID_RB_DEPTH_DOWN;

        if (depth == PE_GRID_GROUP_MAX_DEPTH) {
            pe_grid_flatten_bodies(&VEC_AT(pe_grid.bodies, group->quads[i]));
        } else {
            pe_grid_flatten_group (
                &VEC_AT(pe_grid.groups, group->quads[i]),
                depth + 1
            );
        }

        *VEC_DRY_APPEND(flat_ids_vec) = PE_GRID_RB_DEPTH_UP;
    }
}

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

void pe_grid_init()
{
    // TODO: Choose a real number.
    VEC_INIT(pe_grid.groups, 4);
    VEC_INIT(pe_grid.bodies, 4);
    VEC_INIT(flat_ids_vec, 128);
    pe_grid.first_free_group = PE_GRID_GROUP_ID_NULL;
    pe_grid.first_free_bodies = PE_GRID_BODIES_ID_NULL;

    pe_grid_group_id unused;
    pe_grid_new_group(&unused);
}
