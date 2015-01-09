#include "player.h"

Player
player_init()
{
    Player player = calloc(sizeof(*player), 1);

    ObjectList objlist  = list_from_dir("txt/animations/person");
    objlist->coord = (Coord) {COLS/2, LINES/2};
    player->animation = animation_init(objlist);
    return player;
}

void
player_free(Player player)
{
    animation_free(player->animation);
    free(player);
    player = NULL;
}

void
player_set_rel(Player player, int32_t x, int32_t y)
{
    player->x = x + MAP_X_REL_ZERO - (COLS / 2) + 1;
    player->y = y + MAP_Y_REL_ZERO - (LINES / 2);
}

void
player_set_abs(Player player, int32_t x, int32_t y)
{
    player->x = x - (COLS / 2);
    player->y = y - (LINES / 2);
}

void
player_get_rel(Player player, int32_t *x, int32_t *y)
{
    *x = player->x;
    *y = player->y;
}

void
player_get_abs(Player player, int32_t *x, int32_t *y)
{
    *x = player->x + COLS / 2;
    *y = player->y + LINES / 2;
}

void
player_move(Player player, int32_t x, int32_t y)
{
    player->x = x;
    player->y = y;
}

void
player_animate(Player player, int32_t frame, bool moved)
{
    animation_animate(player->animation, frame, moved);
}

bool
player_can_walk(Player player, Point point)
{
    return true;
}
