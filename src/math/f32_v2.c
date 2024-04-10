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
