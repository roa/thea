#ifndef THEA_SCENEMANAGER_H
#define THEA_SCENEMANAGER_H

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <logger.h>
#include <stdint.h>
#include <sys/time.h>

#include <player.h>
#include <player.h>
#include <point.h>
#include <map.h>
#include <helper.h>
#include <coord.h>
#include <battlemanager.h>

typedef struct SceneManagerData
{
    Map      current_map;
    WINDOW  *win;
    uint64_t scene_x;
    uint64_t scene_y;
    uint32_t frame;
    Player   player;
    bool     moved;
    Coord    old;
    BattleManager bm;
} SceneManagerData;

typedef SceneManagerData *SceneManager;

SceneManager scenemanager_init(WINDOW *win);

void scenemanager_run(SceneManager sm);

void scenemanager_free(SceneManager *sm);

void refresh_screen(SceneManager sm, int delta_x, int delta_y);

void check_for_exit(SceneManager sm);

#endif // THEA_SCENEMANAGER_H
