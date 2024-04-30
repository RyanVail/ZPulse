#ifndef MATH_F32_V2_H
#define MATH_F32_V2_H

#include <common.h>

typedef struct f32_v2 {
    f32 x;
    f32 y;
} f32_v2;

/**
 * Create a f32_v2 with all it's elements being a.
 */
f32_v2 f32_v2_splat(f32 a);

/**
 * Gets the negative of a f32_v2.
 */
f32_v2 f32_v2_neg(f32_v2 a);

/**
 * Add two f32_v2 together. 
 */
f32_v2 f32_v2_add(f32_v2 a, f32_v2 b);

/**
 * Subtracte a f32_v2 from another f32_v2.
 */
f32_v2 f32_v2_sub(f32_v2 a, f32_v2 b);

/**
 * Multipies a f32_v2 by another f32_v2.
 */
f32_v2 f32_v2_mul(f32_v2 a, f32_v2 b);

/**
 * Divide a f32_v2 by another f32_v2.
 */
f32_v2 f32_v2_div(f32_v2 a, f32_v2 b);

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

/**
 * Clamps a f32_v2 between a minimum and maximum.
 */
f32_v2 f32_v2_clamp(f32_v2 a, f32_v2 min, f32_v2 max);

#endif
