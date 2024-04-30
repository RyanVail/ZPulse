#ifndef OBJ_SPRITE_2D_H
#define OBJ_SPRITE_2D_H

#include <render/texture.h>
#include <render/filter.h>
#include <utils/color.h>

/* An id of a 2D sprite. */
typedef u16 o_sprite_id_2d;

/* A 2D sprite. */
typedef struct o_sprite_2d {
    /* The texture of this sprite. */
    r_tex tex;

    /* The filter applied to this sprite's texture. */
    r_tex_filter filter;

    /* The color applied to this sprite's texture. */
    rgba8 color;
} o_sprite_2d;

// TODO: Should this be in the init.c file?
/**
 * Inits the 2D sprites list.
 */
void o_sprite_2d_init();

/**
 * Gets a sprite from a sprite id.
 */
const o_sprite_2d* o_sprite(o_sprite_id_2d id);

/**
 * Loads a 2D sprite.
 * 
 * @param tex The texture to use for the sprite.
 * @param filter The filter to use for the sprite.
 * @param color The color to apply to the sprite.
 * @return The id of the newly loaded sprite.
 */
o_sprite_id_2d o_load_sprite(r_tex tex, r_tex_filter filter, rgba8 color);

#endif
