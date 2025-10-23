#include <game/obj_2d.h>

/* A global list of the 2D objects. */
typeof(g_objs_2d) g_objs_2d;

o_2d* g_add_obj_2d(const o_2d* obj)
{
    VEC_APPEND(g_objs_2d, obj);
    return &VEC_AT(g_objs_2d, g_objs_2d.len - 1);
}

void g_remove_obj_2d(o_2d* obj)
{
    VEC_REMOVE_PTR(g_objs_2d, obj);
}
