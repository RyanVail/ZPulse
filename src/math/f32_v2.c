#include <math/f32_v2.h>
#include <math/f32.h>

f32_v2 f32_v2_splat(f32 a)
{
    return (f32_v2) {
        .x = a,
        .y = a,
    };
}

f32_v2 f32_v2_zero()
{
    return f32_v2_splat(0.0f);
}

f32_v2 f32_v2_neg(f32_v2 a)
{
    return (f32_v2) {
        .x = -a.x,
        .y = -a.y,
    };
}

f32_v2 f32_v2_add(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

f32_v2 f32_v2_sub(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

f32_v2 f32_v2_mul(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x * b.x,
        .y = a.y * b.y,
    };
}

f32_v2 f32_v2_div(f32_v2 a, f32_v2 b)
{
    return (f32_v2) {
        .x = a.x / b.x,
        .y = a.y / b.y,
    };
}

f32_v2 f32_v2_rot(f32_v2 point, f32 sin, f32 cos)
{
    return (f32_v2) {
        .x = point.x * cos - point.y * sin,
        .y = point.x * sin + point.y * cos,
    };
}

f32 f32_v2_mag_sqrd(f32_v2 a)
{
    return (a.x * a.x) + (a.y * a.y);
}

f32 f32_v2_mag(f32_v2 a)
{
    return hypotf(a.x, a.y);
}

f32_v2 f32_v2_normalize(f32_v2 a)
{
    // TODO: Make sure this optimized into an inv sqrt.
    const f32 mag = f32_v2_mag(a);

    /* Making sure there isn't a division by zero. */
    if (mag == 0.0)
        return (f32_v2) { 0, 0 };

    return (f32_v2) {
        .x = a.x / mag,
        .y = a.y / mag,
    };
}

f32_v2 f32_v2_clamp(f32_v2 a, f32_v2 min, f32_v2 max)
{
    return (f32_v2) {
        .x = f32_clamp(a.x, min.x, max.x),
        .y = f32_clamp(a.y, min.y, max.y),
    };
}

f32_v2 f32_v2_scale(f32_v2 a, f32 scalar)
{
    return (f32_v2) {
        .x = a.x * scalar,
        .y = a.y * scalar,
    };
}

bool f32_v2_is_zero(f32_v2 a)
{
    return a.x == 0.0f && a.y == 0.0f;
}
