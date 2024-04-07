#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stddef.h>
#include <inttypes.h>

#include <settings.h>

#define PACKED __attribute__((__packed__))
#define UNUSED __attribute__((__unused__))
#define ALIGN(alignment) __attribute__((aligned(alignment)))
#define UNREACHABLE() __builtin_unreachable()

typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint8_t u8;
typedef int8_t i8;

typedef float f32;
typedef double f64;

#endif
