#include <libs/glfw.h>
#include <render/render.h>
#include <window/glfw.h>
#include <game/game.h>

/**
 * Clears the render buffer.
 */
void r_clear()
{
    // TODO: Make a background color setting.
    glClearColor(0.6f, 0.3f, 0.3f, 1.0f);
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

/**
 * Renders a 2D object.
 */
void r_obj2d(const o_2d* obj)
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

    /* Adjusting the points for the target screen ratio. */
    p0.x *= W_RATIO;
    p1.x *= W_RATIO;

    /* Setting the color of the object. */
    const rgba8 color = o_2d_color(obj);
    glColor4ub(color.r, color.g, color.b, color.a);

    /* Rendering the vertices. */
    glTexCoord2i(0, 0);
    glVertex2f(obj->pos.x + p0.x, obj->pos.y + p0.y);

    glTexCoord2i(1, 0);
    glVertex2f(obj->pos.x + p1.x, obj->pos.y + p1.y);

    glTexCoord2i(1, 1);
    glVertex2f(obj->pos.x - p0.x, obj->pos.y - p0.y);

    glTexCoord2i(0, 1);
    glVertex2f(obj->pos.x - p1.x, obj->pos.y - p1.y);

    glEnd();
}

/**
 * Renders all the 2D objects in the global 2D object list.
 */
void r_objs2d()
{
    for OBJS_2D_ITER(obj)
        r_obj2d(obj);
}
