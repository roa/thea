#ifndef THEA_SCENEMANAGER_H
#define THEA_SCENEMANAGER_H

#include <map.h>
#include <stdio.h>
#include <ncurses.h>
#include <logger.h>
#include <stdint.h>
#include <player.h>

struct SceneManager
{
    Map      *current_map;
    WINDOW   *win;
    int32_t  x;
    int32_t  y;
    int32_t  scene_x;
    int32_t  scene_y;
};

typedef struct SceneManager SceneManager;

void scenemanager_init(SceneManager *sm);
void scenemanager_run(SceneManager *sm);
void scenemanager_free(SceneManager *sm);

void refresh_screen(SceneManager *sm, int delta_x, int delta_y);
bool walkable(Map *map, int _delta_x, int _delta_y);
void check_for_exit(SceneManager *sm);

void move_player_to(SceneManager *sm, int x, int y);

#endif // THEA_SCENEMANAGER_H
