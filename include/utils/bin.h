#ifndef UTILS_BIN_H
#define UTILS_BIN_H

#include <common.h>

/**
 * Reads a big endian encoded `u32` from a set of bytes.
 */
u32 u_read_u32_be(const u8* bytes);

/**
 * Reads a little endian encoded `u32` from a set of bytes.
 */
u32 u_read_u32_le(const u8* bytes);

#endif
