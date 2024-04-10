#include <libs/glfw.h>
#include <libs/stb_image.h>
#include <render/init.h>
#include <render/render.h>
#include <render/texture.h>
#include <window/window.h>
#include <utils/vector.h>
#include <obj/2d.h>
#include <obj/rb_2d.h>
#include <game/game.h>

int main()
{
    r_init();
    o_sprite_2d_init();

    u32 width;
    u32 height;
    UNUSED u32 channels;
    u8* raw = stbi_load (
        "tests/test.tga",
        (i32*)&width,
        (i32*)&height,
        (i32*)&channels,
        0
    );

    if (raw == NULL)
        printf("Failed to load test image");

    r_tex test_tex = r_tex_from_image(raw, width, height);
    o_sprite_id_2d test_sprite = o_load_sprite (
        test_tex,
        R_TEX_FILTER_NONE,
        (rgba8) { 255, 255, 255, 255 }
    );
    stbi_image_free(raw);

    o_2d obj = {
        .pos = { -0.2, -0.2 },
        .scale = { 1 << 7, 1 << 4 },
        .rot = 0,
        .sprite = test_sprite,
        .color = (rgba8) { 255, 255, 255, 255 },
    };

    g_add_obj2d(&obj);

    obj.pos = (f32_v2) {
        .x = 0.7,
        .y = 0.7,
    };

    g_add_obj2d(&obj);

    while (!w_should_close()) {
        r_clear();
        r_objs2d();
        r_flush();
    }

    r_uninit();
}
