#ifndef PHYS_MAT_H
#define PHYS_MAT_H

#include <common.h>

/* An id of a physics material. */
typedef u16 pe_mat_id;

/* A physics material. */
typedef struct pe_mat {
    /* The air resistance of this material. */
    f32 air_res;

    /* The restitution of this material. */
    f32 restitution;

    /* The inverse inertia of this material. */
    f32 inv_inertia;
} pe_mat;

/**
 * Gets a physics material from a physics material id.
 */
const pe_mat* pe_get_mat(pe_mat_id id);

// TODO: There should be a way to remove materials too and these types of add
// functions are inconsitant with their inputs.
/**
 * Adds a material to the global physics material list.
 */
pe_mat_id pe_add_mat(const pe_mat* mat);

#endif
