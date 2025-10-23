#include <game/game.h>
#include <game/obj_2d.h>
#include <game/rb_2d.h>
#include <utils/vector.h>
#include <player/player.h>
#include <game/settings.h>

// TODO: It may make more sense if this was moved into game/player.c
/* The global local player list. */
static VEC(p_player) g_local_players;

// TODO: This could be moved to game/init.c.
void g_init()
{
    VEC_INIT(g_objs_2d, G_OBJ_2D_INIT_LEN);
    VEC_INIT(g_local_players, G_LOCAL_PLAYERS_INIT_LEN);
    VEC_INIT(g_rb_2d_rects, G_RB_2D_INIT_LEN);
    VEC_INIT(g_rb_2d_circles, G_RB_2D_INIT_LEN);
}

p_player* g_add_player()
{
    p_player* player = VEC_DRY_APPEND(g_local_players);
    memset(player, 0, sizeof(p_player));
    return player;
}

void g_tick_players()
{
    for VEC_ITER(g_local_players, player)
        p_tick(player);
}
