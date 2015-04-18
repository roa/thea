src/battlemanager.c:141:            if (!animation_move_towards(bm->player->animation, monster->animation->objlist->coord))
src/battlemanager.c:144:                stats_attack_dmg(bm->player->stats, monster->stats);
src/battlemanager.c:149:            if (!animation_move_towards(bm->player->animation, player_coord))
src/battlemanager.c:152:                bm->player->stats->charged_speed = 0;
src/battlemanager.c:169:                        stats_attack_dmg(monster->stats, bm->player->stats);
src/battlemanager.c:205:    if (!alive || stats_is_dead(bm->player->stats))
src/battlemanager.c:225:        stats_charge_speed(bm->player->stats);
src/battlemanager.c:226:        if (stats_is_turn(bm->player->stats))
src/player.c:10:    player->animation = animation_init(objlist);
src/player.c:11:    player->stats = stats_init();
src/player.c:12:    player->stats->speed     = 2;
src/player.c:13:    player->stats->hitpoints = 1;
src/player.c:20:    animation_free(player->animation);
src/player.c:28:    player->x = x + MAP_X_REL_ZERO - (COLS / 2) + 1;
src/player.c:29:    player->y = y + MAP_Y_REL_ZERO - (LINES / 2);
src/player.c:35:    player->x = x - (COLS / 2);
src/player.c:36:    player->y = y - (LINES / 2);
src/player.c:42:    *x = player->x;
src/player.c:43:    *y = player->y;
src/player.c:49:    *x = player->x + COLS / 2;
src/player.c:50:    *y = player->y + LINES / 2;
src/player.c:56:    player->x = x;
src/player.c:57:    player->y = y;
src/player.c:58:    //player->animation->objlist->coord.x = x;
src/player.c:59:    //player->animation->objlist->coord.y = y;
src/player.c:65:    player->animation->objlist->coord.x = x;
src/player.c:66:    player->animation->objlist->coord.y = y;
src/player.c:72:    animation_animate(player->animation, frame, moved);
src/scenemanager.c:42:                player_move(sm->player, sm->player->x, sm->player->y);
