#ifndef COMMON_H
#define COMMON_H

#define _USE_MATH_DEFINES
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

/* Fixed point values. Encodings are dependent on where it's used. */
typedef u64 fu64;
typedef u32 fu32;
typedef u16 fu16;
typedef u8 fu8;
typedef i64 fi64;
typedef i32 fi32;
typedef i16 fi16;
typedef i8 fi8;

#endif
