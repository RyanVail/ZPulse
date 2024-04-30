#include <math/f32.h>

/**
 * Clamps an f32 between a minimum and maximum.
 */
f32 f32_clamp(f32 a, f32 min, f32 max)
{
    if (a <= min)
        return min;
    else if (a >= max)
        return max;
    return a;
}
