#include <math/u32.h>
#include <debug/debug.h>

u32 u32_mask_bits(u32 bits)
{
    DEBUG_ASSERT (bits < 32,
        "Tried to mask more than 31 bits of a u32."
    );

    return ((u32)1 << bits) - 1;
}
