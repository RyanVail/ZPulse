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
#include <game/obj_2d.h>
#include <game/rb_2d.h>
#include <phys/2d.h>
#include <phys/init.h>
#include <phys/mat.h>

void p_tick(p_player* player)
{
    p_input(player);
    o_rb_2d* rb = player->obj;

    #define ACCEL_SPEED 0.0225f
    #define MAX_SPEED 0.038f
    f32_v2 move = p_normalize_move_input(player);

    rb->vel = f32_v2_add (
        rb->vel,
        f32_v2_mul (
            move,
            (f32_v2) {
                .x = ACCEL_SPEED,
                .y = ACCEL_SPEED,
            }
        )
    );

    rb->vel = f32_v2_clamp (
        rb->vel,
        f32_v2_splat(-MAX_SPEED),
        f32_v2_splat(MAX_SPEED)
    );
}

int main()
{
#if 0
    printf("%lu\n", sizeof(o_2d));
    printf("%lu\n", sizeof(o_rb_2d));
    printf("%lu\n", sizeof(o_rb_2d_circle));
    printf("%lu\n", sizeof(o_rb_2d_rect));
    return 0;
#endif

    r_init_error err = r_init();
    if (err != R_INIT_ERROR_NONE) {
        fprintf(stderr, "Failed to open window.\n");
        exit(-1);
    }

    g_init();
    pe_init();
    o_sprite_2d_init();

    u32 width;
    u32 height;
    UNUSED u32 channels;
    u8* raw = stbi_load (
        "../tests/test.tga",
        (i32*)&width,
        (i32*)&height,
        (i32*)&channels,
        0
    );

    if (raw == NULL)
        printf("Failed to load test image\n");

    r_tex test_tex = r_tex_from_image(raw, width, height);
    o_sprite_id_2d test_sprite = o_load_sprite (
        test_tex,
        R_TEX_FILTER_NONE,
        (rgba8) { 255, 255, 255, 255 }
    );
    stbi_image_free(raw);

    o_rb_2d rb = {
        .obj = {
            .pos = { -0.2, -0.2 },
            .scale = { 1 << 6, 1 << 6 },
            .rot = 0,
            .sprite = test_sprite,
        },
        .vel = { 0.0, 0.0 },
        .ang_vel = 0.02f,
        .inv_mass = 256,
        .mat = 0,
    };

    o_rb_2d_circle* test_circle = VEC_DRY_APPEND(g_rb_2d_circles);
    memcpy(test_circle, &rb, sizeof(rb));
    test_circle->mat = 0;

    test_circle->obj.scale = (fu16_v2) { 1 << 7, 1 << 7 };
    test_circle->obj.pos = (f32_v2) {
        .x = 0.7,
        .y = 0.7,
    };

    const pe_mat player_mat = {
        .air_res = 0.5,
        .restitution = 0.2,
        .inv_inertia = 1.0f / 5.0f,
    };

    const pe_mat_id player_mat_id = pe_add_mat(&player_mat);

    o_rb_2d_circle* player_circle = VEC_DRY_APPEND(g_rb_2d_circles);
    memcpy(player_circle, &rb, sizeof(rb));
    player_circle->ang_vel = 0.0f;
    player_circle->mat = player_mat_id;
    player_circle->vel = (f32_v2) { 0, 0 };
    player_circle->inv_mass = 50;

    p_player* player = g_add_player();
    player->obj = player_circle;
    player->input = (i_input) {
        .type = I_KEYBOARD_AND_MOUSE,
    };

    i_keyboard_load_default(p_keyboard(player));

    // TODO: input, rendering, and ticking should all be done on seperate
    // threads.
    while (!w_should_close()) {
        r_clear();
        pe_2d_tick();
        r_objs_2d();
        g_tick_2d_rbs();
        g_tick_players();
        r_flush();
        //test_circle->obj.rot += 200;
    }

    r_uninit();
}
