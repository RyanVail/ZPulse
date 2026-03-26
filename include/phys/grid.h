#ifndef PHYS_GRID_H
#define PHYS_GRID_H

#include <common.h>
#include <utils/vector.h>
#include <math/f32_v2.h>
#include <game/rb_2d.h>
#include <assert.h>

#define PE_GRID_RB_DEPTH_UP G_RB_2D_ID_NULL
#define PE_GRID_RB_DEPTH_DOWN (G_RB_2D_ID_NULL - 1)

/**
 * A vector of flattened object ids.
 */
typedef VEC(g_rb_2d_id) pe_grid_flat_ids;

#define PE_GRID_TOP_QUAD 0
#define PE_GRID_BOT_QUAD 1
#define PE_GRID_VERT_MASK 1

#define PE_GRID_LEFT_QUAD 2
#define PE_GRID_RIGHT_QUAD 0
#define PE_GRID_HORZ_MASK 2

/* Represents one of the quad children of a group in the physics grid. */
typedef enum pe_grid_quad {
    PE_GRID_QUAD_TOP_LEFT  = PE_GRID_TOP_QUAD | PE_GRID_LEFT_QUAD,
    PE_GRID_QUAD_TOP_RIGHT = PE_GRID_TOP_QUAD | PE_GRID_RIGHT_QUAD,
    PE_GRID_QUAD_BOT_LEFT  = PE_GRID_BOT_QUAD | PE_GRID_LEFT_QUAD,
    PE_GRID_QUAD_BOT_RIGHT = PE_GRID_BOT_QUAD | PE_GRID_RIGHT_QUAD,
} pe_grid_quad;

#define PE_GRID_QUADS 4

static_assert (G_RB_2D_ID_NULL == UINT32_MAX,
    "Physics grid needs G_RB_2D_ID_NULL to be null"
);

/* An id representing a set of bodies in the physics grid. */
typedef u32 pe_grid_bodies_id;

/* An invalid physics bodies set id. */
#define PE_GRID_BODIES_ID_NULL UINT32_MAX

/* A part of a set of bodies in the physics grid. */
typedef struct pe_grid_bodies {
    /*
     * The ids of the rigid bodies within this group. Empty slots are
     * represented with nulls.
     */
    g_rb_2d_id rbs[PE_GRID_GROUP_RB_LEN];

    /*
     * The next group of bodies in this group or null if there aren't any more
     * bodies in this set.
     */
    pe_grid_bodies_id next;
} pe_grid_bodies;

/* An id representing a group in the physics grid. */
typedef u32 pe_grid_group_id;

/* An invalid physics group id. */
#define PE_GRID_GROUP_ID_NULL UINT32_MAX

/* Holds a collection of bodies split into a quad tree. */
typedef struct pe_grid_group {
    /*
     * The indicies of the quads within this group. If this group reached the
     * maximum grid depth these will be sets of bodies, otherwise these are
     * other groups. May contain null values meaning the quad doesn't contain
     * any bodies.
     */
    pe_grid_group_id quads[PE_GRID_QUADS];

    /*
     * The id to the set of bodies within this group, may be null meaning this
     * group doesn't contain any bodies directly.
     */
    pe_grid_bodies_id bodies;
} pe_grid_group;

/*
 * Sadly this is required because `pe_grid_group` uses the same type to
 * represent groups and bodies.
 *
 * @TODO: It could be possible to have another vector parallel to the groups
 * that just stores the first value of the bodies so that a smaller type could
 * be used for the group ids. Another solution would be to reserve some of the
 * early bodies entries for the grid groups, but that may get messy.
 */
static_assert (
    sizeof(pe_grid_group_id) >= sizeof(pe_grid_bodies_id),
    "pe_grid_bodies_id must fit into pe_grid_group_id"
);

/**
 * Gets the grid bodies group for a position and size.
 *
 * @warning All pointers to bodies within the physics grid become invalid after
 * calling this function.
 */
pe_grid_bodies* pe_grid_get_bodies(f32_v2 pos, f32 size);

/**
 * Adds a 2D rigid body to the physics grid.
 */
void pe_grid_rb_2d_add(g_rb_2d_id id, f32_v2 pos, f32 size);

/**
 * If required, moves a 2D rigid body in the physics grid after a size and or a
 * position change.
 */
void pe_grid_rb_2d_move (
    g_rb_2d_id id,
    f32_v2 old_pos,
    f32 old_size,
    f32_v2 new_pos,
    f32 new_size );

/**
 * Flattens the physics quad tree into a single vector.
 *
 * @warning The vector returned by this function is valid up until this
 * function is called again.
 */
const pe_grid_flat_ids* pe_grid_flatten();

/**
 * Cleans unusued structures in the physics grid.
 */
void pe_grid_clean();

#endif
