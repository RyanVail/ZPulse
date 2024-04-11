#ifndef MATH_F32_V2_H
#define MATH_F32_V2_H

#include <common.h>

typedef struct f32_v2 {
    f32 x;
    f32 y;
} f32_v2;

/**
 * Rotates a point represented by a f32_v2 around the origin based on the sin
 * and cos of the angle to rotate by.
 *
 * @param point The position of the point.
 * @param sin The sin of the angle.
 * @param cos The cos of the angle.
 * @return The position of the point rotated around the origin.
 */
f32_v2 f32_v2_rot(f32_v2 point, f32 sin, f32 cos);

/**
 * Gets the magnitude squared of a f32_v2.
 */
f32 f32_v2_mag_sqrd(f32_v2 a);

/**
 * Gets the magnitude of a f32_v2.
 */
f32 f32_v2_mag(f32_v2 a);

/**
 * Normalizes a f32_v2.
 */
f32_v2 f32_v2_normalize(f32_v2 a);

#endif
