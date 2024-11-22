#ifndef PHYS_GRID_H
#define PHYS_GRID_H

#include <common.h>
#include <utils/vector.h>
#include <math/f32_v2.h>
#include <game/rb_2d.h>
#include <assert.h>

/* Ensuring the grid divisions is valid. */
static_assert(PE_GRID_DIVISIONS >= 1, "PE_GRID_DIVISIONS less than one.");

typedef struct pe_grid_obj {
    // TODO: This should be renamed id.
    /* The id of the rb. */
    u32 rb;

    /*
     * The index of the next obj in this division or UINT32_MAX if there's no
     * more in this division.
     */
    u32 next;
} pe_grid_obj;

extern struct {
    VEC(pe_grid_obj) objs;

    /*
     * The start of the null obj linked list in objs or UINT32_MAX if there's
     * no null objects.
     */
    u32 first_empty;

    u32 divisions[1 << (PE_GRID_DIVISIONS * 2)];
} pe_grid;

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
 * Gets the grid division of a position.
 */
u32 pe_grid_division(const f32_v2 pos);

/**
 * Adds a 2D rigid body to the physics grid.
 */
void pe_grid_rb_2d_add(g_rb_2d_id id, u32 division);

/**
 * Removes a 2D rigid body from the physics grid and returns the index of the
 * grid object it use to occupy. The returned index of the grid object must
 * either be added to the null list within the physics grid with
 * pe_grid_rb_2d_null or readded to the physics grid through pe_grid_obj_add.
 */
u32 pe_grid_rb_2d_remove(g_rb_2d_id id, u32 division);

/**
 * Moves a 2D rigid body in the physics grid.
 */
void pe_grid_rb_2d_move(o_rb_2d* rb, u32 old_division, u32 new_division);

/**
 * Adds a rigid body's grid object to the physics grid in the specified
 * division.
 */
void pe_grid_obj_add(u32 grid_obj_index, u32 division);

#endif
