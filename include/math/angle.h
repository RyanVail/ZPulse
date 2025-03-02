#ifndef MATH_ANGLE_H
#define MATH_ANGLE_H

#include <common.h>

/* An angle measure in radians. */
typedef f32 m_angle;

/**
 * Calculates the sin of an angle.
 */
f32 m_sin(m_angle angle);

/**
 * Calculates the cos of an angle.
 */
f32 m_cos(m_angle angle);

#endif
