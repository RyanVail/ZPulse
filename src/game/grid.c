#include <game/grid.h>

f32_v2 g_grid_floor_snap(const g_grid* grid, const f32_v2 point)
{
    return (f32_v2) {
        floorf(point.x / grid->interval.x) * grid->interval.x,
        floorf(point.y / grid->interval.y) * grid->interval.y
    };
}

f32_v2 g_grid_snap(const g_grid* grid, const f32_v2 point)
{
    return (f32_v2) {
        roundf(point.x / grid->interval.x) * grid->interval.x,
        roundf(point.y / grid->interval.y) * grid->interval.y
    };
}
