#ifndef GAME_RB_2D_H
#define GAME_RB_2D_H

#include <obj/rb_2d.h>
#include <utils/vector.h>

/**
 * Adds a rectangle rigid body to the global rectangle list with the default
 * rectangle rigid body struct and any supplied field modifications.
 */
#define G_ADD_RB_RECT(...) \
    g_add_rb_rect ( \
        (const o_rb_2d_rect) { \
            .obj = { \
                .pos = PE_DEFAULT_POS, \
                .scale = PE_DEFAULT_SCALE, \
                .rot = PE_DEFAULT_ROT, \
                .sprite = 0, \
            }, \
            .vel = PE_DEFAULT_VEL, \
            .ang_vel = PE_DEFAULT_ANG_VEL, \
            .inv_mass = PE_DEFAULT_INV_MASS, \
            .mat = 0, \
            __VA_ARGS__ \
        } \
    )

/**
 * Adds a circle rigid body to the global circle list with the default circle
 * rigid body struct and any supplied field modifications.
 */
#define G_ADD_RB_CIRCLE(...) \
    g_add_rb_circle ( \
        (const o_rb_2d_circle) { \
            .obj = { \
                .pos = PE_DEFAULT_POS, \
                .scale = PE_DEFAULT_SCALE, \
                .rot = PE_DEFAULT_ROT, \
                .sprite = 0, \
            }, \
            .vel = PE_DEFAULT_VEL, \
            .ang_vel = PE_DEFAULT_ANG_VEL, \
            .inv_mass = PE_DEFAULT_INV_MASS, \
            .mat = 0, \
            __VA_ARGS__ \
        } \
    )

/*
 * The id attached to a 2D rigid body in the global 2D object list. This id may
 * change when null objects are cleaned.
 */
typedef u32 g_rb_2d_id;

/* The global list of 2D rectangle rigid bodies. */
extern VEC(o_rb_2d_rect) g_rb_2d_rects;

/* The global list of 2D circle rigid bodies. */
extern VEC(o_rb_2d_circle) g_rb_2d_circles;

/**
 * Adds a rectangle rigid body to the global 2D object list.
 */
o_rb_2d_rect* g_add_rb_rect(const o_rb_2d_rect rect);

/**
 * Adds a circle rigid body to the global 2D object list.
 */
o_rb_2d_circle* g_add_rb_circle(const o_rb_2d_circle circle);

/**
 * Gets the type of a 2D rigid body that is inside the global 2D object list.
 */
o_rb_2d_type g_rb_2d_get_type(const o_rb_2d* rb);

// TODO: It might be cheaper to use the first bit of these ids to store the
// type so just a lsr can be used to get rid of it.
/**
 * Gets the id of a 2D rigid body that is inside the global 2D object list.
 *
 * @warning The id of global objects can change when null objects are cleaned.
 */
g_rb_2d_id g_rb_2d_get_id(const o_rb_2d* rb);


/**
 * Ticks all of the 2D rigid bodies.
 */
void g_tick_2d_rbs();

#endif
