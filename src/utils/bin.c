#include <utils/bin.h>

u32 u_read_u32_be(const u8* bytes) {
    return ((u32)bytes[0] << 24)
         | ((u32)bytes[1] << 16)
         | ((u32)bytes[2] << 8)
         | ((u32)bytes[3]);
}

u32 u_read_u32_le(const u8* bytes) {
    return ((u32)bytes[0])
         | ((u32)bytes[1] << 8)
         | ((u32)bytes[2] << 16)
         | ((u32)bytes[3] << 24);
}
