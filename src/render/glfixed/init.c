#include <libs/glfw.h>
#include <window/glfw.h>
#include <render/init.h>

/**
 * Resizes the OpenGL viewport when the window size changes.
 */
void r_framebuffer_resize(UNUSED GLFWwindow* window, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

/**
 * Inits the renderer.
 *
 * @return An error code.
 */
r_init_error r_init()
{
    // TODO: A lot of this stuff with window initializing could be done inside
    // the glfw window file.
    glfwInit();

    glfw_window = glfwCreateWindow (
        W_DEFAULT_WIDTH,
        W_DEFAULT_HEIGHT,
        W_NAME,
        NULL,
        NULL
    );

    if (glfw_window == NULL)
        return R_INIT_ERROR_WINDOW;

    glfwMakeContextCurrent(glfw_window);

    glViewport(0, 0, W_DEFAULT_WIDTH, W_DEFAULT_HEIGHT);
    glfwSetFramebufferSizeCallback(glfw_window, r_framebuffer_resize);

    glEnable(GL_TEXTURE_2D);

    return R_INIT_ERROR_NONE;
}

/**
 * Unitializes the renderer.
 */
void r_uninit()
{
    glfwTerminate();
}
