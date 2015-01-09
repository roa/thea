#ifndef THEA_SCENEMANAGER_H
#define THEA_SCENEMANAGER_H

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <logger.h>
#include <stdint.h>
#include <sys/time.h>

#include <player.h>
#include <animation.h>
#include <player.h>
#include <point.h>
#include <map.h>

typedef struct SceneManagerData
{
    Map      current_map;
    WINDOW  *win;
    uint64_t scene_x;
    uint64_t scene_y;
    int      frame;
    Player   player;
    bool     moved;
} SceneManagerData;

typedef SceneManagerData *SceneManager;

#define INCREMENT_FRAME(_sm)  \
    do {                      \
        if (_sm->frame == 59) \
        {                     \
            _sm->frame = 0;   \
            _sm->moved = 0;   \
        }                     \
        else                  \
            _sm->frame += 1;  \
    }  while (0)

SceneManager scenemanager_init();

void scenemanager_run(SceneManager sm);

void scenemanager_free(SceneManager sm);

void refresh_screen(SceneManager sm, int delta_x, int delta_y);

void check_for_exit(SceneManager sm);

uint64_t get_micro_time();

#endif // THEA_SCENEMANAGER_H
