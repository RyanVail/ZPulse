#include <game/pe_rope.h>

typeof(g_pe_ropes) g_pe_ropes;

pe_rope* g_add_pe_rope(const pe_rope rope)
{
    pe_rope* ptr = VEC_DRY_APPEND(g_pe_ropes);
    memcpy(ptr, &rope, sizeof(rope));
    return ptr;
}
