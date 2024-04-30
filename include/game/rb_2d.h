#ifndef GAME_RB_2D_H
#define GAME_RB_2D_H

#include <obj/rb_2d.h>
#include <utils/vector.h>

// TODO: There should be some functions to add objects to these so it's safe.

/* The global list of 2D rectangle rigid bodies. */
extern VEC(o_rb_2d_rect) g_rb_2d_rects;

/* The global list of 2D circle rigid bodies. */
extern VEC(o_rb_2d_circle) g_rb_2d_circles;

/**
 * Ticks all of the 2D rigid bodies.
 */
void g_tick_2d_rbs();

#endif
