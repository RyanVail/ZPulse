#ifndef PHYS_HINGE_H
#define PHYS_HINGE_H

#include <phys/joint.h>

#define PE_HINGE_ROT()

/**
 * A hinge constraint connecting two rigid bodies.
 */
typedef struct pe_hinge {
    /* The joints within the hinge. */
    pe_joint joints[2];

    /* The length of the hinge constraint between the two joints. */
    f32 length;

    /* The force required to break the hinge. */
    f32 break_force;

    /* The max rotation of the hinge. */
    fu16 max_rot;

    /* The min rotation of the hinge. */
    fu16 min_rot;
} pe_hinge;

/**
 * Calculates a hinge rotation from an angle.
 */
fu16 pe_hinge_rot_from_angle(m_angle a);

#endif
