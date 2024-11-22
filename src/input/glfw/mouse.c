#include <input/mouse.h>
#include <window/glfw.h>

/**
 * Reads input from a mouse.
 */
void i_mouse_read(i_mouse* mouse)
{
    /* Reading the size of the window. */
    int width;
    int height;
    glfwGetWindowSize(glfw_window, &width, &height);

    /* Reading the position of the mouse. */
    double x;
    double y;
    glfwGetCursorPos(glfw_window, &x, &y);
    mouse->pos = (f32_v2) {
        ((f32)x / width - 0.5f) * 2.0f,
        (0.5f - (f32)y / height) * 2.0f
    };

    /* Saving the last inputs. */
    i_mouse_save_last_buttons(mouse);

    /* Reading the button inputs of the mouse. */
    for (i_mouse_button b = 0; b < I_MOUSE_BUTTONS; b++) {
        mouse->buttons[(size_t)b] = glfwGetMouseButton (
            glfw_window,
            (int)b
        ) == GLFW_PRESS;
    }
}
