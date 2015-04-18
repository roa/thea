#ifndef THEA_BATTLEMANAGER_H
#define THEA_BATTLEMANAGER_H

#include <animation.h>
#include <helper.h>
#include <map.h>
#include <gui.h>
#include <logger.h>
#include <menu.h>
#include <player.h>
#include <monster.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MENU_OFFSET 7

#define IDLE           0
#define PLAYER_MOVE_1  1
#define PLAYER_MOVE_2  2
#define CALC_TURN      3
#define MONSTER_TURN_1 4
#define MONSTER_TURN_2 5

typedef struct BattleManagerData
{
    WINDOW      *win;
    GuiMenu      current;
    Map          map;
    Player       player;
    MonsterGroup monster;
    uint32_t     frame;
    bool         moved;
    uint32_t     state;
    uint8_t      index;
    bool         quit;
} BattleManagerData;

typedef BattleManagerData *BattleManager;

BattleManager battlemanager_init(WINDOW *win, Player player);

void battlemanager_run(BattleManager bm);

void battlemanager_free(BattleManager *bm);

void battlemanager_refresh(BattleManager bm);

void battlemanager_clean_menu(BattleManager bm);

void battlemanager_action(BattleManager bm);

void battlemanager_set_state_move(BattleManager);

void process_turn(BattleManager bm, uint32_t frame, bool moved);

void battlemanager_switch_to_attack(BattleManager bm);

void battlemanager_calc_turn(BattleManager bm);

GuiMenu battlemanager_main_menu(BattleManager bm);
GuiMenu battlemanager_attack_menu(BattleManager bm);

#endif // THEA_BATTLEMANAGER_H
