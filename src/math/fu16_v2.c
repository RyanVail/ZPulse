#include <math/fu16_v2.h>

fu16_v2 fu16_v2_splat(fu16 a)
{
    return (fu16_v2) {
        .x = a,
        .y = a,
    };
}
