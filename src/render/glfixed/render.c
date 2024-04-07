#include <libs/glfw.h>
#include <render/render.h>
#include <window/glfw.h>

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
