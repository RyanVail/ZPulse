#include <math/f32.h>

bool f32_is_nan(f32 a)
{
    return isnan(a) != 0;
}

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

/**
 * Calculates the absolute value of an f32.
 */
f32 f32_abs(f32 a)
{
    return fabsf(a);
}

/**
 * Squares an f32 values.
 */
f32 f32_sqr(f32 a)
{
    return a * a;
}

/**
 * Square roots an f32 value.
 */
f32 f32_sqrt(f32 a)
{
    return sqrtf(a);
}
