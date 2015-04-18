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
#include <player.h>
#include <point.h>

#define MAP_WIDTH  60
#define MAP_HEIGHT 40

#define HOUSE_WIDTH  20
#define HOUSE_HEIGHT 10

#define MAP_MAX_X(_width)  (_width + 2 * COLS)
#define MAP_MAX_Y(_height) (_height + 2 * LINES)

#define MAP_X_REL_ZERO (COLS - 1)
#define MAP_Y_REL_ZERO (LINES -1)
#define MAP_X_REL_MAX(_width)  (_width + COLS - 1)
#define MAP_Y_REL_MAX(_height) (_height + LINES - 1)

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
    TOWN,
    DUMMY,
    NUM_MAP_TYPES,
    HOUSE,
    BATTLE
};

typedef struct MapData
{
    Point    **map;
    uint64_t   unique;
    uint32_t   type;
} MapData;

typedef MapData *Map;

Map map_init(uint64_t unique, uint32_t type);

Map sm_map_init(uint32_t scene_x, uint32_t scene_y);

void map_free(Map map);

void map_add_landscape(Map map);

void map_try_add(Map map, const char *dname, int to_add);

bool map_add_object(Map map, const char *obj_name, Coord *p);

bool map_object_fits(Map map, Object obj, Coord *p);

void map_add_exits(Map map);

void map_set_exit(Map map, uint32_t type);

bool walkable(Map map, int _delta_x, int _delta_y);

uint64_t map_get_x_dim(Map map);
uint64_t map_get_y_dim(Map map);

Coord map_create_exit(Map map, uint32_t exit_type);

void map_temp_battle(Map map);

#define EXIT_CHAR 'X'

#endif // THEA_MAP_H_
