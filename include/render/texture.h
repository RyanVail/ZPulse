#ifndef RENDER_TEXTURE
#define RENDER_TEXTURE

#include <common.h>

/* An id assigned to textures. */
typedef u32 r_tex;

// TODO: There should be more options to configure textures like mipmaps,
// offsets, and scales.
// TODO: Maybe texture unloading and loading multiple textures at the same
// time.
/**
 * Creates a texture from an image.
 *
 * @raw The raw data of the image.
 * @param width The width of the image in pixels.
 * @param height The height of the image in pixels.
 * @return The id of the new texture.
 */
r_tex r_tex_from_image(const u8* raw, u32 width, u32 height);

#endif
