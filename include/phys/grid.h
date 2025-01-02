#ifndef PHYS_GRID_H
#define PHYS_GRID_H

#include <common.h>
#include <utils/vector.h>
#include <math/f32_v2.h>
#include <game/rb_2d.h>
#include <assert.h>

#define PE_GRID_RB_DEPTH_UP G_RB_2D_ID_NULL
#define PE_GRID_RB_DEPTH_DOWN (G_RB_2D_ID_NULL - 1)

static_assert (G_RB_2D_ID_NULL == UINT32_MAX,
    "Physics grid needs G_RB_2D_ID_NULL to be null"
);

typedef struct pe_grid_bodies {
    /*
     * The ids of the rigid bodies within this group. May contain nulls for rb
     * ids that don't contain rbs.
     */
    g_rb_2d_id rbs[PE_GRID_GROUP_RB_LEN];

    /*
     * The next group of bodies in this group or *UINT32_MAX* if there aren't
     * any more bodies.
     */
    u32 next_group;
} pe_grid_bodies;

typedef struct pe_grid_group {
    /*
     * The indicies of the groups within this group. If this group reached the
     * maximum group depth these will be sub groups, otherwise these are other
     * groups. May contain *UINT32_MAX* values meaning the group in that quad
     * doesn't contain any bodies.
     */
    u32 groups[4];

    /* The rigid bodies within this group. */
    pe_grid_bodies bodies;
} pe_grid_group;

extern struct {
    VEC(pe_grid_group) groups;
    VEC(pe_grid_bodies) bodies;

    u32 first_empty_group;
    u32 first_empty_bodies;
} pe_grid;

/*
 * A flat version of the physics grid quad tree. *PE_GRID_RB_DEPTH_UP* and
 * *PE_GRID_RB_DEPTH_DOWN* denote changes in depth.
 */
typedef VEC(g_rb_2d_id) pe_grid_flat_ids;

// TODO: There should be another file in the headers called debug.h for debug
// things because this is just bad.
#if DEBUG
#include <render/camera.h>
/**
 * Draws the physics grid for debugging.
 */
void pe_debug_draw_grid(const r_cam* cam);
#endif

/**
 * Gets the grid bodies group for a position and size.
 *
 * @warning All pointers to bodies within the physics grid become invalid after
 * calling this function.
 */
pe_grid_bodies* pe_grid_get_bodies(const f32_v2 pos, f32 size);

/**
 * Adds a 2D rigid body to the physics grid.
 */
void pe_grid_rb_2d_add(g_rb_2d_id id, const f32_v2 pos, f32 size);

/**
 * If required, moves a 2D rigid body in the physics grid after a size and or a
 * position change.
 */
void pe_grid_rb_2d_move (
    g_rb_2d_id id,
    const f32_v2 old_pos,
    f32 old_size,
    const f32_v2 new_pos,
    f32 new_size );

/**
 * Flattens the physics quad tree into a single vector.
 *
 * @warning The vector returned by this function is valid up until this
 * function is called again.
 */
const pe_grid_flat_ids* pe_grid_flatten();

#endif
