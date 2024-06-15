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
#define I_MOUSE_BUTTONS 5

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

#endif
