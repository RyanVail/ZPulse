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
#include <player/player.h>
#include <player/input.h>

void p_tick(p_player* player)
{
    p_input(player);
    o_2d* obj = player->obj;

    #define SPEED 0.05f
    f32_v2 move = p_normalize_move_input(player);
    obj->pos.x += move.y * SPEED * W_RATIO;
    obj->pos.y += move.x * SPEED;
}

int main()
{
    r_init_error err = r_init();
    if (err != R_INIT_ERROR_NONE) {
        fprintf(stderr, "Failed to open window.");
        exit(-1);
    }

    g_init();
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
        .scale = { 1 << 6, 1 << 6 },
        .rot = 0,
        .sprite = test_sprite,
        .color = (rgba8) { 255, 255, 255, 255 },
    };

    o_2d* test_obj = g_add_obj2d(&obj);

    obj.scale = (fu16_v2) { 1 << 7, 1 << 7 };
    obj.pos = (f32_v2) {
        .x = 0.7,
        .y = 0.7,
    };

    o_2d* test_obj_ptr = g_add_obj2d(&obj);
    p_player* player = g_add_player();
    player->obj = test_obj_ptr;
    player->input = (i_input) {
        .type = I_KEYBOARD_AND_MOUSE,
    };

    i_keyboard_load_default(p_keyboard(player));

    // TODO: input, rendering, and ticking should all be done on seperate
    // threads.
    while (!w_should_close()) {
        r_clear();
        r_objs2d();
        g_tick_players();
        r_flush();
        test_obj->rot += 200;
    }

    r_uninit();
}
