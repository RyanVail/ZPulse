#include <game/game.h>
#include <player/player.h>

// TODO: These should be moved into another file like game/obj_2d.c.
typeof(objs_2d) objs_2d;

/* The global local player list. */
static VEC(p_player) local_players;

/**
 * Inits the game.
 */
void g_init()
{
    VEC_INIT(objs_2d, 1024);
    VEC_INIT(local_players, 8);
}

/**
 * Adds a 2D object to the global 2D object list.
 */
o_2d* g_add_obj2d(const o_2d* obj)
{
    VEC_APPEND(objs_2d, obj);
    return &VEC_AT(objs_2d, objs_2d.len - 1);
}

/**
 * Removes a 2D object from the global 2D object list.
 */
void g_remove_obj2d(o_2d* obj)
{
    VEC_REMOVE_PTR(objs_2d, obj);
}

/**
 * Adds a player to the global local player list.
 */
p_player* g_add_player()
{
    p_player* player = VEC_DRY_APPEND(local_players);
    memset(player, 0, sizeof(p_player));
    return player;
}

/**
 * Ticks all players in the local player list.
 */
void g_tick_players()
{
    for VEC_ITER(local_players, player)
        p_tick(player);
}
