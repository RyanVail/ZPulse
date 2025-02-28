#ifndef PE_GAME_ROPE_H
#define PE_GAME_ROPE_H

#include <phys/settings.h>
#include <phys/rope.h>
#include <utils/vector.h>
#include <common.h>

/**
 * Adds a rope constraint to the global constraint list with the default rope
 * struct and any supplied field modifications.
 *
 * @warning The *bodies* field must be defined as it's not defined by default.
 */
#define G_ADD_PE_ROPE(...) \
    g_add_pe_rope ( \
        (const pe_rope) { \
            .length = PE_ROPE_DEFAULT_LENGTH, \
            .strength = PE_ROPE_DEFAULT_STRENGTH, \
            .break_force = PE_ROPE_DEFAULT_BREAK_FORCE, \
            __VA_ARGS__ \
        } \
    )

/* An id of a rope constraint. */
typedef u32 g_pe_rope_id;

/* The global list of rope constraints. */
extern VEC(pe_rope) g_pe_ropes;

/**
 * Adds a rope to the global rope constraint list.
 */
pe_rope* g_add_pe_rope(const pe_rope rope);

#endif
