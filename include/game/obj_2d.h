#ifndef GAME_OBJ_2D_H
#define GAME_OBJ_2D_H

#include <obj/2d.h>
#include <utils/vector.h>

/* A global list of the 2D objects. */
extern VEC(o_2d) g_objs_2d;

/**
 * Adds a 2D object to the global 2D object list.
 */
o_2d* g_add_obj_2d(const o_2d* obj);

/**
 * Removes a 2D object from the global 2D object list.
 */
void g_remove_obj_2d(o_2d* obj);

#endif
