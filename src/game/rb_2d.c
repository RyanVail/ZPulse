#include <game/rb_2d.h>

/* The global list of 2D rectangle rigid bodies. */
typeof(g_rb_2d_rects) g_rb_2d_rects;

/* The global list of 2D circle rigid bodies. */
typeof(g_rb_2d_circles) g_rb_2d_circles;

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
