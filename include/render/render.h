#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H

#include <obj/2d.h>

/**
 * Clears the render buffer.
 */
void r_clear();

/**
 * Flushes the render buffer.
 */
void r_flush();

/**
 * Renders a 2D object.
 */
void r_obj2d(const o_2d* obj);

/**
 * Renders all the 2D objects in the global 2D object list.
 */
void r_objs2d();

#endif
