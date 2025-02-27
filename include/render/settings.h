#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

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
