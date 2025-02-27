#ifndef MATH_F32_H
#define MATH_F32_H

#include <common.h>

/**
 * Clamps an f32 between a minimum and maximum.
 */
f32 f32_clamp(f32 a, f32 min, f32 max);

/**
 * Calculates the absolute value of an f32.
 */
f32 f32_abs(f32 a);

/**
 * Squares an f32 value.
 */
f32 f32_sqr(f32 a);

/**
 * Square roots an f32 value.
 */
f32 f32_sqrt(f32 a);

#endif
