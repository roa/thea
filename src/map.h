#ifndef THEA_MAP_H_
#define THEA_MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ncurses.h>

#include <object.h>
#include <helper.h>
#include <logger.h>
#include <animation.h>
#include <point.h>
#include <map_exit.h>

#define MAP_WIDTH  300
#define MAP_HEIGHT 150

#define MAP_MAX_X (MAP_WIDTH + 2 * COLS)
#define MAP_MAX_Y (MAP_HEIGHT + 2 * LINES)

#define MAP_X_REL_ZERO (COLS - 1)
#define MAP_Y_REL_ZERO (LINES -1)
#define MAP_X_REL_MAX (MAP_WIDTH + COLS - 1)
#define MAP_Y_REL_MAX (MAP_HEIGHT + LINES - 1)

#define SCENE_MAX_X 2
#define SCENE_MAX_Y 2

#define SAFE_SPACE 5

#define CREATION_TRIES 100

/*
 *  corresponding position relative to zero value on shown area
 *  on screen
 */
#define X_REL(_offset, _iter) _offset + _iter

#define Y_REL(_offset, _iter) _offset + _iter

/*
 * position on MAP
 */
#define MAP_X_REL(x) (COLS + x)
#define MAP_Y_REL(y) (LINES + y)

enum Map_Types {
    town,
    not_town,
    NUM_MAP_TYPES
};

typedef struct MapData
{
    Point    **map;
    ExitList   exits;
    uint64_t   unique;
    uint64_t   scene_x;
    uint64_t   scene_y;
    int        type;
} MapData;

typedef MapData *Map;

Map map_init(uint64_t scene_x, uint64_t scene_y);

void map_set_type(Map map);

void map_free(Map map);

void map_add_landscape(Map map);

void map_try_add(Map map, const char *dname, int to_add);

bool map_add_object(Map map, const char *obj_name, Coord *p);

bool map_object_fits(Map map, Object obj, Coord *p);

void map_add_exits(Map map);

void map_debug(Map map);

#define EXIT_CHAR 'X'

#define CREATE_UNIQUE(_unique, _x, _y) \
    do {                               \
        _unique = 0;                   \
        _unique = _unique | _x;        \
        _unique = _unique | _y << 32;  \
    } while (0)

#endif // THEA_MAP_H_
