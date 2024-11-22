#include <phys/grid.h>
#include <math/u32.h>

/* The global grid. */
typeof(pe_grid) pe_grid;

#if DEBUG
#include <render/render.h>
/**
 * Draws the physics grid for debugging.
 */
void pe_debug_draw_grid(const r_cam* cam)
{
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
}
#endif

/**
 * Gets the grid division of a position.
 */
u32 pe_grid_division(const f32_v2 pos)
{
    const f32 grid_scale = (f32)(1 << PE_GRID_DIVISIONS) / PE_GRID_SIZE;
    const f32_v2 grid_pos = f32_v2_mul(pos, f32_v2_splat(grid_scale));

    const u32 div_mask = u32_mask_bits(PE_GRID_DIVISIONS);
    return ((i32)grid_pos.x & div_mask)
        | (((i32)grid_pos.y & div_mask) << PE_GRID_DIVISIONS);
}

/**
 * Adds a 2D rigid body to the physics grid.
 */
void pe_grid_rb_2d_add(g_rb_2d_id id, u32 division)
{
    const u32 grid_obj_index = pe_grid.objs.len;
    pe_grid_obj* grid_obj = VEC_DRY_APPEND(pe_grid.objs);
    grid_obj->rb = id;
    grid_obj->next = pe_grid.divisions[division];
    pe_grid.divisions[division] = grid_obj_index;
}

/**
 * Removes a 2D rigid body from the physics grid and returns the index of the
 * grid object it use to occupy. The returned index of the grid object must
 * either be added to the null list within the physics grid with
 * pe_grid_rb_2d_null or readded to the physics grid through pe_grid_obj_add.
 */
u32 pe_grid_rb_2d_remove(g_rb_2d_id id, u32 division)
{
    /* If the id is the first link in the linked list. */
    u32 obj_index = pe_grid.divisions[division];
    pe_grid_obj* obj = &VEC_AT(pe_grid.objs, obj_index);
    if (obj->rb == id) {
        pe_grid.divisions[division] = obj->next;
        return obj_index;
    }

    /* Searching for the id of the rigid body. */
    while (true) {
        DEBUG_ASSERT (obj_index != UINT32_MAX,
            "Couldn't find object id when removing from physics grid."
        );

        obj = &VEC_AT(pe_grid.objs, obj_index);

        DEBUG_ASSERT (obj->next != UINT32_MAX,
            "Couldn't find object id when removing from physics grid."
        );

        /*
         * If the id of the rigid body was found remove it from the linked
         * list.
         */
        obj_index = obj->next;
        const pe_grid_obj* next_obj = &VEC_AT(pe_grid.objs, obj_index);
        if (next_obj->rb == id) {
            obj->next = next_obj->next;
            break;
        }
    }

    return obj_index;
}

/**
 * Moves a 2D rigid body in the physics grid.
 */
void pe_grid_rb_2d_move(o_rb_2d* rb, u32 old_division, u32 new_division)
{
    DEBUG_ASSERT (old_division != new_division,
        "Tried to move a 2D rigid body to and from the same division."
    );

    // TODO: This could be done in the ticking function since it would just
    // basically be the index of the current rigid body being ticked which can
    // already be calculated and maybe just iterate using the index of the rb
    // since it's aligned to 32 bytes anyway.
    const g_rb_2d_id id = g_rb_2d_get_id(rb);

    const u32 obj_index = pe_grid_rb_2d_remove(id, old_division);
    pe_grid_obj_add(obj_index, new_division);
}

/**
 * Adds a rigid body's grid object to the physics grid in the specified
 * division.
 */
void pe_grid_obj_add(u32 grid_obj_index, u32 division)
{
    /* Adding the grid object before the old start index of the division. */
    VEC_AT(pe_grid.objs, grid_obj_index).next = pe_grid.divisions[division];
    pe_grid.divisions[division] = grid_obj_index;
}

/**
 * Inits the physics engine's grid.
 */
void pe_grid_init()
{
    // TODO: Choose a real number.
    VEC_INIT(pe_grid.objs, 16);
    pe_grid.first_empty = UINT32_MAX;
    memset(pe_grid.divisions, 255, sizeof(pe_grid.divisions));
}
