#include <game/rb_2d.h>
#include <phys/grid.h>

/* The global list of 2D rectangle rigid bodies. */
typeof(g_rb_2d_rects) g_rb_2d_rects;

/* The global list of 2D circle rigid bodies. */
typeof(g_rb_2d_circles) g_rb_2d_circles;

// TODO: These functions shouldn't have _get_ in their names.
/**
 * Gets the type of a rigid body that is inside the global 2D object list.
 */
o_rb_2d_type g_rb_2d_get_type(const o_rb_2d* rb)
{
    if (VEC_IS_PTR_WITHIN(g_rb_2d_rects, rb))
        return O_RB_2D_RECT;

    DEBUG_ASSERT (VEC_IS_PTR_WITHIN(g_rb_2d_circles, rb),
        "Tried to get the type of a rigid body that isn't in the global 2D "
        "object list."
    );

    return O_RB_2D_CIRCLE;
}

/**
 * Gets the id of a 2D rigid body that is inside the global 2D object list.
 *
 * @warning The id of global objects can change when null objects are cleaned.
 */
g_rb_2d_id g_rb_2d_get_id(const o_rb_2d* rb)
{
    return (g_rb_2d_get_type(rb) == O_RB_2D_CIRCLE)
        ? (g_rb_2d_id)((1u << 31) | VEC_PTR_INDEX(g_rb_2d_circles, rb))
        : (g_rb_2d_id)(VEC_PTR_INDEX(g_rb_2d_rects, rb));
}

/**
 * Adds a circle rigid body to the global 2D object list.
 */
o_rb_2d_circle* g_add_rb_circle(const o_rb_2d_circle circle)
{
    // TODO: There should probably be another function that can be called when
    // rigid bodies are moved like what's done in the ticking function and
    // there should be warning in the rigid body struct.
    pe_grid_rb_2d_add (
        (1u << 31) | g_rb_2d_circles.len,
        pe_grid_division(circle.obj.pos)
    );

    o_rb_2d_circle* ptr = VEC_DRY_APPEND(g_rb_2d_circles);
    memcpy(ptr, &circle, sizeof(circle));
    return ptr;
}

/**
 * Adds a rectangle rigid body to the global 2D object list.
 */
o_rb_2d_rect* g_add_rb_rect(const o_rb_2d_rect rect)
{
    pe_grid_rb_2d_add(g_rb_2d_rects.len, pe_grid_division(rect.obj.pos));
    o_rb_2d_rect* ptr = VEC_DRY_APPEND(g_rb_2d_rects);
    memcpy(ptr, &rect, sizeof(rect));
    return ptr;
}

/**
 * Ticks all of the 2D rigid bodies.
 */
void g_tick_2d_rbs()
{
    for VEC_ITER(g_rb_2d_rects, rect)
        o_rb_2d_rect_tick(rect);
    for VEC_ITER(g_rb_2d_circles, circle)
        o_rb_2d_circle_tick(circle);
}
