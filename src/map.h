#ifndef THEA_MAP_H_
#define THEA_MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ncurses.h>

#include <object.h>
#include <logger.h>

#define MAP_MAX_X 500
#define MAP_MAX_Y 100

#define SCENE_MAX_X 2
#define SCENE_MAX_Y 2

#define SAFE_SPACE 5

#define MAYBE_CONTINUE(_chance)       \
        if (random() % 100 < _chance) \
            continue;

struct Point
{
    int x;
    int y;
};

typedef struct Point Point;

/*
 *  corresponding zero value on screen
 */
#define SCREEN_X (MAP_MAX_X / 2 + COLS / 2)
#define SCREEN_Y (MAP_MAX_Y / 2 + LINES / 2)

/*
 *  corresponding position relative to zero value on shown area
 *  on screen
 */
#define X_REL(_offset, _iter) SCREEN_X + _offset + _iter
#define Y_REL(_offset, _iter) SCREEN_Y + _offset + _iter

/*
 * position on MAP
 */
#define MAP_X_REL(x) (COLS + x)
#define MAP_Y_REL(y) (LINES + y)

struct Map
{
    char  **map;
    bool  **walkable;
    int     max_x;
    int     max_y;
    int     exits[4];
    int     x_rel_zero;
    int     x_rel_max;
    int     y_rel_zero;
    int     y_rel_max;
    int64_t unique;
};

typedef struct Map Map;

void map_init(Map **map, int screen_width, int screen_height, int64_t scene_x, int64_t scene_y);

void map_free(Map **map);

void map_add_houses(Map **map);

bool map_add_object(Map **map, const char *obj_name, Point *p);

bool map_object_fits(Map **map, Object *obj, Point *p);

void map_debug(Map *map);

#define EXIT_CHAR 'X'

/* TODO set seed for int64
 * hint: replace 16 with 32 and find right function
 */
#define CREATE_UNIQUE(_unique, _x, _y) \
    do {                               \
        _unique = 0;                   \
        _unique = _unique | _x;        \
        _unique = _unique | _y << 16;  \
    } while (0)

#define SET_EXIT_1(_map, _x, _y)                                                        \
    do {                                                                                \
        if (_x == 0) break;                                                             \
        _map->exits[0] = random() % MAP_MAX_Y;                                          \
        _map->map[_map->exits[0] + _map->y_rel_zero][_map->x_rel_zero] = EXIT_CHAR;     \
        _map->map[_map->exits[0] + _map->y_rel_zero + 1][_map->x_rel_zero] = EXIT_CHAR; \
        _map->map[_map->exits[0] + _map->y_rel_zero - 1][_map->x_rel_zero] = EXIT_CHAR; \
        _map->walkable[_map->exits[0] + _map->y_rel_zero][_map->x_rel_zero] = true;     \
        _map->walkable[_map->exits[0] + _map->y_rel_zero + 1][_map->x_rel_zero] = true; \
        _map->walkable[_map->exits[0] + _map->y_rel_zero - 1][_map->x_rel_zero] = true; \
    } while (0)

#define SET_EXIT_2(_map, _x, _y)                                                        \
    do {                                                                                \
        if (_x == SCENE_MAX_X - 1) break;                                               \
        _map->exits[1] = random() % MAP_MAX_Y;                                          \
        _map->map[_map->exits[1] + _map->y_rel_zero][_map->x_rel_max] = EXIT_CHAR;      \
        _map->map[_map->exits[1] + _map->y_rel_zero + 1][_map->x_rel_max] = EXIT_CHAR;  \
        _map->map[_map->exits[1] + _map->y_rel_zero - 1][_map->x_rel_max] = EXIT_CHAR;  \
        _map->walkable[_map->exits[1] + _map->y_rel_zero][_map->x_rel_max] = true;      \
        _map->walkable[_map->exits[1] + _map->y_rel_zero + 1][_map->x_rel_max] = true;  \
        _map->walkable[_map->exits[1] + _map->y_rel_zero - 1][_map->x_rel_max] = true;  \
    } while (0)

#define SET_EXIT_3(_map, _x, _y)                                                        \
    do {                                                                                \
        if (_y == 0) break;                                                             \
        _map->exits[2] = random() % MAP_MAX_Y;                                          \
        _map->map[_map->y_rel_zero][_map->exits[2] + _map->x_rel_zero] = EXIT_CHAR;     \
        _map->map[_map->y_rel_zero][_map->exits[2] + _map->x_rel_zero + 1] = EXIT_CHAR; \
        _map->map[_map->y_rel_zero][_map->exits[2] + _map->x_rel_zero - 1] = EXIT_CHAR; \
        _map->walkable[_map->y_rel_zero][_map->exits[2] + _map->x_rel_zero] = true;     \
        _map->walkable[_map->y_rel_zero][_map->exits[2] + _map->x_rel_zero + 1] = true; \
        _map->walkable[_map->y_rel_zero][_map->exits[2] + _map->x_rel_zero - 1] = true; \
    } while (0)

#define SET_EXIT_4(_map, _x, _y)                                                        \
    do {                                                                                \
        if (_y == SCENE_MAX_Y - 1) break;                                               \
        _map->exits[3] = random() % MAP_MAX_Y;                                          \
        _map->map[_map->y_rel_max][_map->exits[3] + _map->x_rel_zero] = EXIT_CHAR;      \
        _map->map[_map->y_rel_max][_map->exits[3] + _map->x_rel_zero + 1] = EXIT_CHAR;  \
        _map->map[_map->y_rel_max][_map->exits[3] + _map->x_rel_zero - 1] = EXIT_CHAR;  \
        _map->walkable[_map->y_rel_max][_map->exits[3] + _map->x_rel_zero] = true;      \
        _map->walkable[_map->y_rel_max][_map->exits[3] + _map->x_rel_zero + 1] = true;  \
        _map->walkable[_map->y_rel_max][_map->exits[3] + _map->x_rel_zero - 1] = true;  \
    } while (0)

#define SET_EXITS(_map, _x, _y)   \
    do {                          \
        SET_EXIT_1(_map, _x, _y); \
        SET_EXIT_2(_map, _x, _y); \
        SET_EXIT_3(_map, _x, _y); \
        SET_EXIT_4(_map, _x, _y); \
    } while (0)

#define TOUCH_EXIT_1(_sm) \
    ((_sm->current_map->x_rel_zero == LEFT_ARM_X + X_REL(0, _sm->x)) && (_sm->current_map->y_rel_zero + _sm->current_map->exits[0] == LEFT_ARM_Y + Y_REL(0, _sm->y)))

#define TOUCH_EXIT_2(_sm) \
    ((_sm->current_map->x_rel_max == RIGHT_ARM_X + X_REL(0, _sm->x)) && (_sm->current_map->y_rel_zero + _sm->current_map->exits[1] == RIGHT_ARM_Y + Y_REL(0, _sm->y)))

#define TOUCH_EXIT_3(_sm) \
    ((_sm->current_map->x_rel_zero + _sm->current_map->exits[2] == HEAD_X + X_REL(0, _sm->x)) && (_sm->current_map->y_rel_zero == HEAD_Y + Y_REL(0, _sm->y)))

#define TOUCH_EXIT_4(_sm) \
    ((_sm->current_map->x_rel_zero + _sm->current_map->exits[3] == RIGHT_FOOT_X + X_REL(0, _sm->x)) && (_sm->current_map->y_rel_max == RIGHT_FOOT_Y + Y_REL(0, _sm->y)))

#endif // THEA_MAP_H_
