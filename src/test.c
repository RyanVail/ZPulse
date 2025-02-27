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
#include <game/grid.h>
#include <player/player.h>
#include <player/input.h>
#include <game/obj_2d.h>
#include <game/rb_2d.h>
#include <phys/2d.h>
#include <phys/init.h>
#include <phys/mat.h>
#include <phys/grid.h>
#include <render/camera.h>
#include <math/f32.h>
#include <window/settings.h>

static r_cam cam;

static const g_grid grid = {
    .interval = (const f32_v2) { 0.25f, 0.25f },
};

static o_2d* obj = NULL;
static f32_v2 circle_start;
static o_sprite_id_2d mouse_circle_id;

void p_tick(p_player* player)
{
    p_input(player);
    o_rb_2d* rb = player->obj;

    const f32_v2 mouse_pos = g_grid_snap (
        &grid,
        r_cam_pos_to_game_pos(&cam, p_mouse(player)->pos)
    );

    if (player->secondary_attacking) {
        if (obj == NULL || circle_start.x == FLT_MAX) {
            circle_start = mouse_pos;
            const o_2d _obj = {
                .pos = mouse_pos,
                .scale = { 0, 0 },
                .rot = 0,
                .sprite = mouse_circle_id,
            };

            obj = g_add_obj_2d(&_obj);
        } else {
            const f32 dist = f32_v2_mag(f32_v2_sub(circle_start, mouse_pos));
            obj->scale.x = (i32)(f32_abs(dist) * 711.111111f);
        }
    } else {
        if (obj != NULL && circle_start.x != FLT_MAX) {
            G_ADD_RB_CIRCLE (
                .obj.pos = obj->pos,
                .obj.scale = obj->scale,
                .obj.sprite = mouse_circle_id,
                .inv_mass = 2048,
            );

            g_remove_obj_2d(obj);
        }

        circle_start.x = FLT_MAX;
    }

    #define ACCEL_SPEED 0.0225f
    #define MAX_SPEED 0.038f
    const f32_v2 move = p_normalize_move_input(player);
    rb->vel = f32_v2_add(rb->vel, f32_v2_mul(move, f32_v2_splat(ACCEL_SPEED)));
    rb->vel = f32_v2_clamp (
        rb->vel,
        f32_v2_splat(-MAX_SPEED),
        f32_v2_splat(MAX_SPEED)
    );
}

void test_repel (
    const o_rb_2d* repel_circle,
    const o_rb_2d* player_circle,
    const p_player* player,
    const i_mouse* mouse )
{
    for VEC_ITER(g_rb_2d_circles, c) {
#if 0

        c->vel = f32_v2_add (
            c->vel, 
            f32_v2_mul (
                f32_v2_splat(0.0025f),
                f32_v2_normalize(f32_v2_sub(mouse_pos, c->obj.pos))
            )
        );
#endif

        if (c->mat == player_circle->mat || c == repel_circle)
            continue;

        const f32_v2 mouse_pos = r_cam_pos_to_game_pos(&cam, mouse->pos);
        const f32_v2 dif = f32_v2_sub(c->obj.pos, mouse_pos);
        if (player->primary_attacking) {
            c->vel = f32_v2_sub (
                c->vel, 
                f32_v2_mul (
                    f32_v2_splat(0.00285f),
                    f32_v2_normalize(dif)
                )
            );
        }
    }
}

void test_player_line(const o_rb_2d_circle* player_circle)
{
#if DEBUG
    r_line (
        &cam,
        player_circle->obj.pos,
        (const f32_v2) { 0.0f, 0.0f },
        (const rgba8) { 0, 0, 0, 0 }
    );
#endif

}

int main()
{
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
        fprintf(stderr, "Failed to load test image\n");

    cam = (r_cam) {
        .pos = { 0.0f, 0.0f },
        .zoom = { 1.0f * W_RATIO, 1.0f },
    };

    r_tex test_tex = r_tex_from_image(raw, width, height);
    o_sprite_id_2d test_sprite = O_ADD_SPRITE (
        .tex = test_tex,
        .color = (const rgba8) { 255, 128, 255, 0 },
    );
    stbi_image_free(raw);

    mouse_circle_id = O_ADD_SPRITE (
        .color = (const rgba8) { 255, 128, 128, 0 },
    );

    /* Creating a bunch of tiny test circles. */
    for (u32 i = 0; i < 256; i++) {
        UNUSED o_rb_2d_circle* circle = G_ADD_RB_CIRCLE (
            .obj.pos = (f32_v2) {
                (i & 31) * 0.0425f - 0.7f,
                (i >> 5) * 0.0425f - 0.7f
            },
            .obj.scale = (fu16_v2) { 1 << 3, 1 << 3 },
            .inv_mass = 65535,
        );
    }

    /* Making a larger circle that repels other circles. */
    o_rb_2d_circle* repel_circle = G_ADD_RB_CIRCLE (
        .obj.pos = (f32_v2) { 0.7f, 0.7f },
        .obj.scale = (fu16_v2) { 1 << 5, 1 << 5 },
        .inv_mass = 8,
    );

    const pe_mat player_mat = {
        .air_res = 0.5f,
        .restitution = 0.2f,
        .inv_inertia = 1.0f / 5.0f,
    };

    const pe_mat_id player_mat_id = pe_add_mat(&player_mat);

    o_rb_2d_circle* player_circle = G_ADD_RB_CIRCLE (
        .obj.pos = { -0.9f, -0.5f },
        .obj.scale = { 1 << 6, 1 << 6 },
        .obj.sprite = test_sprite,
        .inv_mass = 0,
        .mat = player_mat_id,
    );

    p_player* player = g_add_player();
    player->obj = player_circle;
    player->input = (i_input) {
        .type = I_KEYBOARD_AND_MOUSE,
    };

    i_keyboard_load_default(p_keyboard(player));
    i_mouse_load_default(p_mouse(player));

    // TODO: input, rendering, and ticking should all be done on seperate
    // threads.
    while (!w_should_close()) {
        r_clear();
        pe_debug_draw_grid(&cam);
        test_repel(repel_circle, player_circle, player, p_mouse(player));
        pe_2d_tick();
        g_tick_2d_rbs();
        g_tick_players();
        cam.pos = player_circle->obj.pos;
        test_player_line(player_circle);
        r_objs_2d(&cam);
        r_flush();
    }

    r_uninit();
}
