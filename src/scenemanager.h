#ifndef THEA_SCENEMANAGER_H
#define THEA_SCENEMANAGER_H

#include <map.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <logger.h>
#include <stdint.h>
#include <player.h>
#include <sys/time.h>
#include <animation.h>
#include <player.h>
#include <point.h>

#define PLAYER_X (sm->x + COLS/2)
#define PLAYER_Y (sm->y + LINES/2)

typedef struct SceneManagerData
{
    Map      current_map;
    WINDOW  *win;
    int32_t  x;
    int32_t  y;
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
bool walkable(Map map, int _delta_x, int _delta_y);
void check_for_exit(SceneManager sm);

void move_player_to_rel(SceneManager sm, int x, int y);
void move_player_to_abs(SceneManager sm, int x, int y);

uint64_t get_micro_time();

#endif // THEA_SCENEMANAGER_H
