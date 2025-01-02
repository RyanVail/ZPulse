#include <math/fu16_v2.h>

/**
 * Create a fu16_v2 with all it's elements being a.
 */
fu16_v2 fu16_v2_splat(fu16 a)
{
    return (fu16_v2) {
        .x = a,
        .y = a,
    };
}
