#ifndef SETTINGS_H
#define SETTINGS_H

/* Debug settings. */
#define DEBUG true

/* Window settings. */
#define W_NAME "ZPulse"
#define W_DEFAULT_WIDTH 1920
#define W_DEFAULT_HEIGHT 1080
#define W_RATIO (9.0f / 16.0f) // TODO: It would make more sense if this were
                               // to be reversed.

/* Input settings. */
#define I_MOUSE_BUTTONS 8

/* Game engine settings. */
#define MULTITHREADED true
#define G_OBJ_2D_INIT_LEN 512
#define G_RB_2D_INIT_LEN 128
#define G_LOCAL_PLAYERS_INIT_LEN 8

/* 2D physics engine settings. */
#define PE_2D_PAIRS_INIT_LEN 1024
#define PE_MATS_INIT_LEN 16
#define PE_DEFAULT_AIR_RES 0.02f
#define PE_DEFAULT_RESTITUTION 0.85f
#define PE_DEFAULT_INERTIA 5.0f
#define PE_DEFAULT_INV_MASS 8192
#define PE_DEFAULT_POS { 0.0f, 0.0f }
#define PE_DEFAULT_SCALE { 16, 16 }
#define PE_DEFAULT_ROT 0
#define PE_DEFAULT_VEL { 0.0f, 0.0f }
#define PE_DEFAULT_ANG_VEL 0.0f
#define PE_GRID_GROUP_MAX_DEPTH 64
#define PE_GRID_GROUP_RB_LEN 64
#define PE_GRID_SIZE 256

/* Rendering settings. */
#define R_SPRITE_DEFAULT_SHAPE R_SHAPE_CIRCLE
#define R_SPRITE_DEFAULT_TEX 0
#define R_SPRITE_DEFAULT_FILTER R_TEX_FILTER_NONE
#define R_SPRITE_DEFAULT_COLOR { 0, 0, 0, 0 }

/* Fixed render settings. */
#define R_CIRCLE_POINTS 32
#define R_CIRCLE_ROTATION true
#define R_LINE_THICKNESS 0.05f

/* Making sure the circle points are valid. */
#if (R_CIRCLE_POINTS & 1) != 0 || R_CIRCLE_POINTS <= 2
    #error "Invalid cirlce render points"
#endif

#endif
