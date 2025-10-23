#include <game/obj_2d.h>
#include <game/settings.h>

/* A global list of the 2D objects. */
typeof(g_objs_2d) g_objs_2d;

void g_objs_2d_init()
{
    VEC_INIT(g_objs_2d, G_OBJ_2D_INIT_LEN);
}

o_2d* g_add_obj_2d(const o_2d* obj)
{
    VEC_APPEND(g_objs_2d, obj);
    return &VEC_AT(g_objs_2d, g_objs_2d.len - 1);
}

void g_remove_obj_2d(o_2d* obj)
{
    VEC_REMOVE_PTR(g_objs_2d, obj);
}
