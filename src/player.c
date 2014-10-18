#include "player.h"

Player
player_init()
{
    Player player = calloc(sizeof(*player), 1);

    ObjectList objlist  = list_from_dir("txt/animations/person");
    objlist->coord = (Coord) {COLS/2, LINES/2};
    player->animation = animation_init(objlist);
    player->frame = 0;
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
player_animate(Player player, int32_t frame, bool moved)
{
    animation_animate(player->animation, frame, moved);
}
