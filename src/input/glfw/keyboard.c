#include <input/keyboard.h>
#include <window/glfw.h>

/**
 * Reads input from a keyboard.
 */
void i_keyboard_read(i_keyboard* keyboard)
{
    /* Saving the last keys. */
    i_keyboard_save_last_keys(keyboard);

    /* Getting the letter key inputs. */
    for (i_key i = I_KEY_A; i <= I_KEY_Z; i++) {
        keyboard->keys[(size_t)i] = glfwGetKey (
            glfw_window,
            (int)i + (GLFW_KEY_A - (int)I_KEY_A)
        );
    }
}
