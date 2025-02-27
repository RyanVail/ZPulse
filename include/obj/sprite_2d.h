// TODO: THis should probably be under render/texture.

#ifndef OBJ_SPRITE_2D_H
#define OBJ_SPRITE_2D_H

#include <render/settings.h>
#include <render/texture.h>
#include <render/filter.h>
#include <render/shape.h>
#include <utils/color.h>

/**
 * Adds a sprite to the global sprite list with the default sprite struct and
 * any supplied field modifications. Returns the id the sprite is represented
 * by.
 */
#define O_ADD_SPRITE(...) \
    o_add_sprite ( \
        (const o_sprite_2d) { \
            .shape = R_SPRITE_DEFAULT_SHAPE, \
            .tex = R_SPRITE_DEFAULT_TEX, \
            .filter = R_SPRITE_DEFAULT_FILTER, \
            .color = R_SPRITE_DEFAULT_COLOR, \
            __VA_ARGS__ \
        } \
    )

/* An id of a 2D sprite. */
typedef u16 o_sprite_id_2d;

/* A 2D sprite. */
typedef struct o_sprite_2d {
    /* The shape of this sprite. */
    r_shape shape;

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
 * Adds a sprite to the global 2D sprite list and returns the id that the
 * newely added sprite is represented by.
 */
o_sprite_id_2d o_add_sprite(const o_sprite_2d sprite);

#endif
