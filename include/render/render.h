#ifndef RENDER_RENDER_H
#define RENDER_RENDER_H

#include <obj/2d.h>
#include <render/camera.h>

/**
 * Clears the render buffer.
 */
void r_clear();

/**
 * Flushes the render buffer.
 */
void r_flush();

/**
 * Renders a circle object to a camera.
 */
void r_circle(const r_cam* cam, const o_2d* obj);

/**
 * Renders a rectangle object to a camera.
 */
void r_rect(const r_cam* cam, const o_2d* obj);

/**
 * Renders a line to a camera.
 */
void r_line(const r_cam* cam, f32_v2 start, f32_v2 end, const rgba8 color);

/**
 * Renders a 2D object to a camera.
 */
void r_obj_2d(const r_cam* cam, const o_2d* obj);

/**
 * Renders all the 2D objects in the global 2D object list and all the objects
 * of the rigid bodies in the 2D global object lists to a camera.
 */
void r_objs_2d(const r_cam* cam);

#endif
