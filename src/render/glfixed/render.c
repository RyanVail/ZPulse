#include <libs/glfw.h>
#include <render/render.h>
#include <render/camera.h>
#include <window/glfw.h>
#include <game/obj_2d.h>
#include <game/rb_2d.h>
#include <assert.h>
#include <math/angle.h>
#include <math/f32.h>

void r_clear()
{
    // TODO: Make a background color setting.
    glClearColor(0.16f, 0.16f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void r_flush()
{
    glfwSwapBuffers(glfw_window);
    glfwPollEvents();
}

static f32_v2 r_circle_points[R_CIRCLE_POINTS];

void r_circle_init()
{
    /* Increments to increase the angle of points by. */
    const f32 inc = (f32)(M_PI * 2.0 / (f64)(R_CIRCLE_POINTS));

    const f32_v2 one = f32_v2_splat(1.0f);

    m_angle angle = 0;
    for (size_t i = 0; i < R_CIRCLE_POINTS; i++) {
        angle += inc;
        r_circle_points[i] = f32_v2_rot(one, m_sin(angle), m_cos(angle));
    }
}

// TODO: Implement textures again.
void r_circle(const r_cam* cam, const o_2d* obj)
{
    const f32_v2 size = o_2d_size_f32_v2(obj);

    /* Don't render if this circle isn't in frame. */
    if (!r_cam_circle_in_fov(cam, obj->pos, size.x))
        return;

    glBindTexture(GL_TEXTURE_2D, o_2d_tex(obj));
    glBegin(GL_POLYGON);

    /* Setting the color of the object. */
    const rgba8 color = o_2d_color(obj);
    glColor4ub(color.r, color.g, color.b, color.a);

    for (size_t i = 0; i < ARRAY_LEN(r_circle_points); i++) {
        const f32_v2 point = r_game_pos_to_cam_pos (
            cam,
            f32_v2_fmadd(r_circle_points[i], size, obj->pos)
        );

        glVertex2f(point.x, point.y);
    }

    glEnd();
}

void r_rect(const r_cam* cam, const o_2d* obj)
{
    const f32_v2 size = o_2d_size_f32_v2(obj);

    /* Don't render if this rectangle isn't in frame. */
    const f32 approx_width = f32_max(size.x, size.y);
    if (!r_cam_rect_in_fov(cam, obj->pos, f32_v2_splat(approx_width)))
        return;

    /* Used to rotate the points around based on the objects rotation. */
    const f32 s = o_2d_rot_sin(obj);
    const f32 c = o_2d_rot_cos(obj);

    glBindTexture(GL_TEXTURE_2D, o_2d_tex(obj));
    glBegin(GL_QUADS);

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

void r_objs_2d(const r_cam* cam)
{
    for VEC_ITER(g_objs_2d, obj)
        r_obj_2d(cam, obj);
    for VEC_ITER(g_rb_2d_rects, obj)
        r_obj_2d(cam, &obj->obj);
    for VEC_ITER(g_rb_2d_circles, obj)
        r_obj_2d(cam, &obj->obj);
}
