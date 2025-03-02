#ifndef PHYS_JOINT_H
#define PHYS_JOINT_H

#include <game/rb_2d.h>

/**
 * A joint within a physics constraint.
 */
typedef struct pe_joint {
    /* The body the joint is attached to. */
    g_rb_2d_id rb;

    /* The offset from the body to the position of the joint. */
    f32_v2 offset;
} pe_joint;

#endif
