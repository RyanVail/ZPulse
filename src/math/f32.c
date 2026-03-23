#include <math/f32.h>

bool f32_is_nan(f32 a)
{
    return isnan(a) != 0;
}

f32 f32_clamp(f32 a, f32 min, f32 max)
{
    if (a <= min)
        return min;
    else if (a >= max)
        return max;
    return a;
}

f32 f32_abs(f32 a)
{
    return fabsf(a);
}

f32 f32_max(f32 a, f32 b)
{
    return fmaxf(a, b);
}

f32 f32_sqr(f32 a)
{
    return a * a;
}

f32 f32_sqrt(f32 a)
{
    return sqrtf(a);
}
