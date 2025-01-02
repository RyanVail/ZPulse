#include <utils/color.h>

/**
 * Blends to colors together.
 */
rgba8 rgba8_blend(const rgba8 a, const rgba8 b)
{
    return (rgba8) {
        .r = (u8)(((u16)a.r + (u16)b.r) >> 1),
        .g = (u8)(((u16)a.g + (u16)b.g) >> 1),
        .b = (u8)(((u16)a.b + (u16)b.b) >> 1),
        .a = (u8)(((u16)a.a + (u16)b.a) >> 1),
    };
}
