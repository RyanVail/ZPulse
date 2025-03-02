#include <phys/hinge.h>

fu16 pe_hinge_rot_from_angle(m_angle a)
{
    return ((m_angle)obj->rot / 65535.0f) * (m_angle)(2 * M_PI);
}
