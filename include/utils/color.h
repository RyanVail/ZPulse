#ifndef UTILS_COLOR_H
#define UTILS_COLOR_H

#include <common.h>

// TODO: This should be moved to render.
typedef struct rgba8 {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} rgba8;

/**
 * Blends two colors together.
 */
rgba8 rgba8_blend(const rgba8 a, const rgba8 b);

#endif
