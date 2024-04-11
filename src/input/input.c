#include <input/input.h>
#include <debug/debug.h>

/**
 * Reads input from an input processor.
 */
void i_read(i_input* processor)
{
    DEBUG_ASSERT (processor != NULL,
        "Tried to read from an input processor that's NULL."
    );

    switch (processor->type)
    {
    case I_KEYBOARD_AND_MOUSE:
        i_keyboard_read(&processor->processor.keyboard_and_mouse.keyboard);
        // TODO: Add mouse input here.
        //i_mouse_read(processor->processor.keyboard_and_mouse.mouse);
        break;
    case I_CONTROLLER:
        // TODO: Controller read.
        break;

    default:
        UNREACHABLE();
        break;
    }
}
