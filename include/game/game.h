#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <obj/2d.h>
#include <utils/vector.h>

/* A global list of the 2D objects. */
extern VEC(o_2d) objs_2d;

/**
 * The body of a for loop that iterates over every element in the global 2D
 * object list.
 *
 * @param obj2d_name The name to give each 2D object.
 */
#define OBJS_2D_ITER(obj2d_name) VEC_ITER(objs_2d, obj2d_name)

/**
 * Adds a 2D object to the global 2D object list.
 */
o_2d* g_add_obj2d(const o_2d* obj);

/**
 * Removes a 2D object from the global 2D object list.
 */
void g_remove_obj2d(o_2d* obj);

#endif
