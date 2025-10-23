#include <game/player.h>
#include <utils/vector.h>
#include <game/settings.h>

/* The global local player list. */
static VEC(p_player) g_local_players;

void g_players_init()
{
    VEC_INIT(g_local_players, G_LOCAL_PLAYERS_INIT_LEN);
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
