#include <window/glfw.h>
#include <common.h>

/* The global window controlled by GLFW. */
GLFWwindow* glfw_window;

/**
 * Checks if the window is trying to be closed.
 */
bool w_should_close()
{
    return glfwWindowShouldClose(glfw_window);
}
