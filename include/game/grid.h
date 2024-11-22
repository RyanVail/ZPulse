#ifndef GAME_GIRD_H
#define GAME_GRID_H

#include <math/f32_v2.h>

/* A representation of a grid. */
typedef struct g_grid {
    /* The interval points snap to on the grid. */
    const f32_v2 interval;
} g_grid;

/**
 * Snaps a point to a grid by flooring it.
 */
f32_v2 g_grid_floor_snap(const g_grid* grid, const f32_v2 point);

/**
 * Snaps a point to a grid.
 */
f32_v2 g_grid_snap(const g_grid* grid, const f32_v2 point);

#endif
