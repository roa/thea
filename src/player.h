#ifndef THEA_PLAYER_H
#define THEA_PLAYER_H

#include <stdbool.h>
#include <stdint.h>
#include <animation.h>
#include <logger.h>
#include <object.h>

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
} PlayerData;

typedef PlayerData *Player;

Player player_init();
void player_free(Player player);
void player_animate(Player player, int32_t frame, bool moved);

#endif // THEA_PLAYER_H
