#include <libs/glfw.h>
#include <render/render.h>
#include <render/camera.h>
#include <window/glfw.h>
#include <game/obj_2d.h>
#include <game/rb_2d.h>
#include <assert.h>

/**
 * Clears the render buffer.
 */
void r_clear()
{
    // TODO: Make a background color setting.
    glClearColor(0.16f, 0.16f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * Flushes the render buffer
 */
void r_flush()
{
    glfwSwapBuffers(glfw_window);
    glfwPollEvents();
}

// TODO: Implement textures again.
/**
 * Renders a circle object to a camera.
 */
void r_circle(const r_cam* cam, const o_2d* obj)
{
    glBindTexture(GL_TEXTURE_2D, o_2d_tex(obj));
    glBegin(GL_POLYGON);

    const f32_v2 size = f32_v2_mul (
        f32_v2_splat(0.72f),
        o_2d_size_f32_v2(obj)
    );

    // TODO: This should just calculate them twice in the render loop.
    /* Half of the points surrounding the circle. */
    static f32_v2 points[R_CIRCLE_POINTS / 2];

    /* Incraments to increase the angle of points by. */
    const f32 inc = (f32)M_PI / (R_CIRCLE_POINTS / 2);

    /*
     * Rotating the points of the circle based on the rotation of the object.
     */
#if R_CIRCLE_ROTATION
    f32 ang = o_2d_ang(obj);
#else
    f32 ang = 0.0;
#endif

    /* Caluclating the locations of the points. */
    for (size_t i = 0; i < ARRAY_LEN(points); i++) {
        ang += inc;
        points[i] = f32_v2_rot(size, sinf(ang), cosf(ang));
    }

    /* Setting the color of the object. */
    const rgba8 color = o_2d_color(obj);
    glColor4ub(color.r, color.g, color.b, color.a);

    /* Redering the top half. */
    for (size_t i = 0; i < ARRAY_LEN(points); i++) {
        const f32_v2 point = f32_v2_mul (
            cam->zoom,
            f32_v2_sub (
                obj->pos,
                f32_v2_add(points[i], cam->pos)
            )
        );

        glVertex2f(point.x, point.y);
    }

    /* Redering the bottom half. */
    for (size_t i = 0; i < ARRAY_LEN(points); i++) {
        const f32_v2 point = f32_v2_mul (
            cam->zoom,
            f32_v2_add (
                obj->pos,
                f32_v2_sub(points[i], cam->pos)
            )
        );

        glVertex2f(point.x, point.y);
    }

    glEnd();
}

/**
 * Renders a rectangle object to a camera.
 */
void r_rect(const r_cam* cam, const o_2d* obj)
{
    /* Used to rotate the points around based on the objects rotation. */
    const f32 s = o_2d_rot_sin(obj);
    const f32 c = o_2d_rot_cos(obj);

    glBindTexture(GL_TEXTURE_2D, o_2d_tex(obj));
    glBegin(GL_QUADS);

    const f32_v2 size = o_2d_size_f32_v2(obj);

    // TODO: Is there to get all four points with a single point?
    /* Calculating two diagonal points. */
    f32_v2 p0 = f32_v2_rot((f32_v2) { size.x, size.y }, s, c);
    f32_v2 p1 = f32_v2_rot((f32_v2) { -size.x, size.y }, s, c);

    /* Setting the color of the object. */
    const rgba8 color = o_2d_color(obj);
    glColor4ub(color.r, color.g, color.b, color.a);

    /* Rendering the vertices. */
    glTexCoord2i(0, 0);
    glVertex2f (
        (obj->pos.x + p0.x - cam->pos.x) * cam->zoom.x,
        (obj->pos.y + p0.y - cam->pos.y) * cam->zoom.y
    );

    glTexCoord2i(1, 0);
    glVertex2f (
        (obj->pos.x + p1.x - cam->pos.x) * cam->zoom.x,
        (obj->pos.y + p1.y - cam->pos.y) * cam->zoom.y
    );

    glTexCoord2i(1, 1);
    glVertex2f (
        (obj->pos.x - p0.x - cam->pos.x) * cam->zoom.x,
        (obj->pos.y - p0.y - cam->pos.y) * cam->zoom.y
    );

    glTexCoord2i(0, 1);
    glVertex2f (
        (obj->pos.x - p1.x - cam->pos.x) * cam->zoom.x,
        (obj->pos.y - p1.y - cam->pos.y) * cam->zoom.y
    );

    glEnd();
}

/**
 * Renders a line to a camera.
 */
void r_line(const r_cam* cam, f32_v2 start, f32_v2 end, const rgba8 color)
{
    /* Shifting the start and end by the camera's position and zoom. */
    start = f32_v2_mul(f32_v2_sub(start, cam->pos), cam->zoom);
    end = f32_v2_mul(f32_v2_sub(end, cam->pos), cam->zoom);

    glColor4ub(color.r, color.g, color.b, color.a);
    glBegin(GL_LINE_STRIP);
    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    glEnd();
}

/**
 * Renders a 2D object to a camera.
 */
void r_obj_2d(const r_cam* cam, const o_2d* obj)
{
    switch (o_2d_sprite(obj)->shape)
    {
    case R_SHAPE_CIRCLE:
        r_circle(cam, obj);
        break;
    case R_SHAPE_RECT:
        r_rect(cam, obj);
        break;
    default:
        UNREACHABLE();
        break;
    }
}

/**
 * Renders all the 2D objects in the global 2D object list and all the objects
 * of the rigid bodies in the 2D global object lists to a camera.
 */
void r_objs_2d(const r_cam* cam)
{
    for VEC_ITER(g_objs_2d, obj)
        r_obj_2d(cam, obj);
    for VEC_ITER(g_rb_2d_rects, obj)
        r_obj_2d(cam, &obj->obj);
    for VEC_ITER(g_rb_2d_circles, obj)
        r_obj_2d(cam, &obj->obj);
}
