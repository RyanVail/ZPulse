#ifndef DEBUG_DEBUG_H
#define DEBUG_DEBUG_H

#include <common.h>

// TODO: This should be doing something a little less destructive than crashing
// the program.
/**
 * Ensures a condition is true at runtime. If the condition is false this will
 * print the __VA_ARGS__ inputs, flush all outputs, and crash the program.
 */
#define ASSERT(condition, ...) \
    if (!(condition)) { \
        fprintf(stderr, __VA_ARGS__); \
        fflush(stderr); \
        fflush(stdout); \
        abort(); \
    }

#if DEBUG
    #define DEBUG_ASSERT(condition, ...) ASSERT(condition, __VA_ARGS__)
#else
    #define DEBUG_ASSERT(condition, ...) if (!(condition)) UNREACHABLE()
#endif

#endif
