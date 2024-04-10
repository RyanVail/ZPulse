#include <game/game.h>

// TODO: These should be moved into another file like game/obj_2d.c.
typeof(objs_2d) objs_2d;

/**
 * Adds a 2D object to the global 2D object list.
 */
o_2d* g_add_obj2d(const o_2d* obj)
{
    VEC_APPEND(objs_2d, obj);
    return &VEC_AT(objs_2d, objs_2d.len - 1);
}

/**
 * Removes a 2D object from the global 2D object list.
 */
void g_remove_obj2d(o_2d* obj)
{
    VEC_REMOVE_PTR(objs_2d, obj);
}
