#ifndef MATH_FU32_V2_H
#define MATH_FU32_V2_H

#include <common.h>

typedef struct fu16_v2 {
    fu16 x;
    fu16 y;
} fu16_v2;

/**
 * Create a fu16_v2 with all it's elements being a.
 */
fu16_v2 fu16_v2_splat(fu16 a);

#endif
