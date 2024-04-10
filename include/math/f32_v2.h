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

#endif
