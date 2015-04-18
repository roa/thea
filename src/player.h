#ifndef THEA_PLAYER_H
#define THEA_PLAYER_H

#include <stdbool.h>
#include <stdint.h>

#include <animation.h>
#include <coord.h>
#include <logger.h>
#include <map.h>
#include <object.h>
#include <point.h>
#include <stats.h>

#define HEAD_Y          LINES/2
#define HEAD_X          COLS/2+1
#define BODY_Y          LINES/2+1
#define BODY_X          COLS/2
#define LEFT_FOOT_Y     LINES/2+2
#define LEFT_FOOT_X     COLS/2+1
#define RIGHT_FOOT_Y    LEFT_FOOT_Y
#define RIGHT_FOOT_X    COLS/2+2
#define LEFT_ARM_Y      BODY_Y
#define LEFT_ARM_X      COLS/2-1
#define RIGHT_ARM_Y     BODY_Y
#define RIGHT_ARM_X     COLS/2+1

typedef struct PlayerData {
    Animation animation;
    int32_t   frame;
    int32_t   x;
    int32_t   y;
    Stats     stats;
} PlayerData;

typedef PlayerData *Player;

Player player_init();

Player player_init_from_stats(Stats stats);

void player_free(Player player);

void player_set_rel(Player player, int32_t x, int32_t y);

void player_set_abs(Player player, int32_t x, int32_t y);

void player_get_rel(Player player, int32_t *x, int32_t *y);

void player_get_abs(Player player, int32_t *x, int32_t *y);

void player_move(Player player, int32_t x, int32_t y);

void player_battle_move(Player player, int32_t x, int32_t y);

void player_animate(Player player, int32_t frame, bool moved);

bool player_can_walk(Player player, Point point);

Coord player_battle_pos();

#endif // THEA_PLAYER_H
