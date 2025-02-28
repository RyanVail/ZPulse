#include <math/f32_v2.h>
#include <math/f32.h>

/**
 * Create a f32_v2 with all it's elements being a.
 */
f32_v2 f32_v2_splat(f32 a)
{
    return (f32_v2) {
        .x = a,
        .y = a,
    };
}

/**
 * Create a f32_v2 with all it's elements being 0.
 */
f32_v2 f32_v2_zero()
{
    return f32_v2_splat(0.0f);
}

/**
 * Gets the negative of a f32_v2.
 */
f32_v2 f32_v2_neg(f32_v2 a)
{
    return (f32_v2) {
        .x = -a.x,
        .y = -a.y,
    };
}

/**
 * Add two f32_v2 together. 
 */
f32_v2 f32_v2_add(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

/**
 * Subtracted a f32_v2 from another f32_v2.
 */
f32_v2 f32_v2_sub(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

/**
 * Multipies a f32_v2 by another f32_v2.
 */
f32_v2 f32_v2_mul(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x * b.x,
        .y = a.y * b.y,
    };
}

/**
 * Divide a f32_v2 by another f32_v2.
 */
f32_v2 f32_v2_div(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x / b.x,
        .y = a.y / b.y,
    };
}

/**
 * Rotates a point represented by a f32_v2 around the origin based on the sin
 * and cos of the angle to rotate by.
 *
 * @param point The position of the point.
 * @param sin The sin of the angle.
 * @param cos The cos of the angle.
 * @return The position of the point rotated around the origin.
 */
f32_v2 f32_v2_rot(f32_v2 point, f32 sin, f32 cos)
{
    return (f32_v2) {
        .x = point.x * cos - point.y * sin,
        .y = point.x * sin + point.y * cos,
    };
}

/**
 * Gets the magnitude squared of a f32_v2.
 */
f32 f32_v2_mag_sqrd(f32_v2 a)
{
    return (a.x * a.x) + (a.y * a.y);
}

/**
 * Gets the magnitude of a f32_v2.
 */
f32 f32_v2_mag(f32_v2 a)
{
    return hypotf(a.x, a.y);
}

/**
 * Normalizes a f32_v2.
 */
f32_v2 f32_v2_normalize(f32_v2 a)
{
    // TODO: Make sure this optimized into an inv sqrt.
    const f32 mag = f32_v2_mag(a);

    /* Making sure there isn't a division by zero. */
    if (mag == 0.0)
        return (f32_v2) { 0, 0 };

    return (f32_v2) {
        .x = a.x / mag,
        .y = a.y / mag,
    };
}

/**
 * Clamps a f32_v2 between a minimum and maximum.
 */
f32_v2 f32_v2_clamp(f32_v2 a, f32_v2 min, f32_v2 max)
{
    return (f32_v2) {
        .x = f32_clamp(a.x, min.x, max.x),
        .y = f32_clamp(a.y, min.y, max.y),
    };
}

/**
 * Scales a f32_v2 by a scalar value.
 */
f32_v2 f32_v2_scale(f32_v2 a, f32 scalar)
{
    return (f32_v2) {
        .x = a.x * scalar,
        .y = a.y * scalar,
    };
}

/**
 * Checks if all elements in a f32_v2 are zeros.
 */
bool f32_v2_is_zero(f32_v2 a)
{
    return a.x == 0.0f && a.y == 0.0f;
}
