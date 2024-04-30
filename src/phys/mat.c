#include <phys/mat.h>
#include <utils/vector.h>

/* The global physics material list. */
VEC(pe_mat) pe_mats;

/**
 * Inits the physics engine's material list.
 */
void pe_mat_init()
{
    VEC_INIT(pe_mats, PE_MATS_INIT_LEN);

    const pe_mat mat = {
        .air_res = PE_DEFAULT_AIR_RES,
        .restitution = PE_DEFAULT_RESTITUTION,
        .inv_inertia = 1.0f / PE_DEFAULT_INERTIA,
    };

    VEC_APPEND(pe_mats, &mat);
}

/**
 * Gets a physics material from a physics material id.
 */
const pe_mat* pe_get_mat(pe_mat_id id)
{
    return &VEC_AT(pe_mats, (size_t)id);
}

/**
 * Adds a material to the global physics material list.
 */
pe_mat_id pe_add_mat(const pe_mat* mat)
{
    VEC_APPEND(pe_mats, mat);
    return (pe_mat_id)(pe_mats.len - 1);
}
