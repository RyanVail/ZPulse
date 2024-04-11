#include <math/f32_v2.h>

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
