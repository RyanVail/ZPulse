#include <phys/init.h>

/**
 * Inits the physics engine.
 */
void pe_init()
{
    pe_2d_init();
    pe_mat_init();
    pe_grid_init();
}
