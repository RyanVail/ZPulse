#include <obj/sprite_2d.h>
#include <utils/vector.h>

// TODO: All of these sprite things could just be moved to world.
// TODO: There has to be a way to remove sprites too. And that should free the
// texture made by OpenGL.
/* The sprites currently loaded. */
static VEC(o_sprite_2d) _2d_sprites;

/**
 * Inits the 2D sprites list.
 */
void o_sprite_2d_init()
{
    VEC_INIT(_2d_sprites, 16);
}

/**
 * Gets a sprite from a sprite id.
 */
const o_sprite_2d* o_sprite(o_sprite_id_2d id)
{
    return &VEC_AT(_2d_sprites, id);
}

/**
 * Loads a 2D sprite.
 * 
 * @param tex The texture to use for the sprite.
 * @param filter The filter to use for the sprite.
 * @param color The color to apply to the sprite.
 * @return The id of the newly loaded sprite.
 */
o_sprite_id_2d o_load_sprite(r_tex tex, r_tex_filter filter, rgba8 color)
{
    o_sprite_2d* sprite = VEC_DRY_APPEND(_2d_sprites);
    *sprite = (o_sprite_2d) {
        .tex = tex,
        .filter = filter,
        .color = color,
    };

    return _2d_sprites.len - 1;
}
