#include "map.h"
#include "logger.h"

/***********************************************
 * naming:
 *
 *  _________________
 * |   ___________   |
 * |  |           |  |
 * |  |           |  |
 * |  |___________|  |
 * |_________________|
 *
 * |                     x is zero
 * <--|                  value of x_rel_zero
 *    |<--------->|      MAP_WIDTH
 *                |-->   value of x_rel_zero
 * <----------------->   map->max_x
 *
 * same for y values
 * y = 0 is on the top
 *
 ***********************************************/

Map
map_init(uint64_t unique, uint32_t type)
{
    uint64_t map_max_x,
             map_max_y;

    Map map = calloc(sizeof(*map), 1);
    map->unique = unique;
    map->type   = type;

    switch (map->type)
    {
        case TOWN:
        case DUMMY:
        default:
            map_max_x = MAP_MAX_X;
            map_max_y = MAP_MAX_Y;
    }

    map->map = calloc(sizeof(Point), map_max_y);

    for (uint64_t y = 0; y < map_max_y; ++y)
    {
        map->map[y] = calloc(sizeof(Point), map_max_x);
        for (uint64_t x = 0; x < map_max_x; ++x)
        {
            map->map[y][x] = point_init();
            if (x == MAP_X_REL_ZERO || x == MAP_X_REL_MAX)
                point_set(map->map[y][x], '?', 0);
            else if (y == MAP_Y_REL_ZERO || y == MAP_Y_REL_MAX)
                point_set(map->map[y][x], '?', 0);
            else
                point_set(map->map[y][x], ' ', WALKABLE);
        }
    }
    map_add_exits(map);
    map_add_landscape(map);
    return map;
}

void
map_add_landscape(Map map)
{
    switch(map->type)
    {
        case TOWN:
            logger_log("town");
            map_try_add(map, "txt/town", 4);
            map_try_add(map, "txt/flora/tree", 100);
            break;
        case DUMMY:
            logger_log("DUMMY");
            break;
        default:
            logger_log("unknown");
    }
}

void
map_try_add(Map map, const char *dname, int to_add)
{
    int added = 0;
    Coord p = {MAP_X_REL(1), MAP_Y_REL(1)};
    for(int tries = 0; tries < CREATION_TRIES && added < to_add; ++tries)
        for (int local_y = p.y; local_y< MAP_Y_REL_MAX; ++local_y)
        {
            MAYBE_CONTINUE(99);
            for (int local_x = p.x; local_x < MAP_X_REL_MAX; ++local_x )
            {
                MAYBE_CONTINUE(99);
                Coord local_p = {local_x, local_y};
                char *obj_name = dir_globbing(dname);
                if (map_add_object(map, obj_name, &local_p))
                    added++;
                free(obj_name);
            }
        }
}

void
map_free(Map map)
{
    for (int y = 0; y < MAP_MAX_Y; ++y)
        for (int x = 0; x < MAP_MAX_X; ++x)
            point_free(map->map[y][x]);

    for (int y = 0; y < MAP_MAX_Y; ++y)
        free(map->map[y]);

    free(map->map);
    map->map = NULL;
    free(map);
    map = NULL;
}

bool
map_add_object(Map map, const char *obj_name, Coord *p)
{
    int local_x = p->x;
    int local_y = p->y;

    Object obj = object_init(obj_name);

    if (!map_object_fits(map, obj, p))
    {
        object_free(obj);
        return false;
    }

    for (int i = 0; i < obj->used; ++i)
    {
        if (point_get_content(obj->object[i]) == '\n')
        {
            ++local_y;
            local_x = p->x;
            continue;
        }
        if (point_get_content(obj->object[i]) != ' ')
            point_set(map->map[local_y][local_x++], point_get_content(obj->object[i]), point_get_type(obj->object[i]));
        else
            point_set(map->map[local_y][local_x++], point_get_content(obj->object[i]), WALKABLE);
    }

    object_free(obj);
    return true;
}

bool
map_object_fits(Map map, Object obj, Coord *p)
{
    for (int y = p->y - SAFE_SPACE; y < p->y + obj->height + SAFE_SPACE; ++y)
        for (int x = p->x - SAFE_SPACE; x < p->x + obj->width + SAFE_SPACE; ++x)
            if (point_get_content(map->map[y][x]) != ' ')
                return false;
    return true;
}

Coord
map_create_exit(Map map, uint32_t exit_type)
{
    Coord exit;
    srandom(map->unique);
    switch (exit_type)
    {
        case LEFT_EXIT:
            exit.x = MAP_X_REL_ZERO;
            exit.y = random() % MAP_HEIGHT + MAP_Y_REL_ZERO;
            break;
        case RIGHT_EXIT:
            exit.x = MAP_X_REL_MAX;
            exit.y =  random() % MAP_HEIGHT + MAP_Y_REL_ZERO;
            break;
        case UPPER_EXIT:
            exit.x = random() % MAP_WIDTH + MAP_X_REL_ZERO;
            exit.y = MAP_Y_REL_ZERO;
            break;
        case LOWER_EXIT:
            exit.x = random() % MAP_WIDTH + MAP_X_REL_ZERO;
            exit.y = MAP_Y_REL_MAX;
    }
    return exit;
}

void
map_add_exits(Map map)
{
    // Left exit
    if (CREATE_SCENE_X(map->unique) != 0)
    {
        Coord exit = map_create_exit(map, LEFT_EXIT);
        for (int8_t i = -2; i <= 2; ++i)
        {
            point_set(map->map[exit.y+i][exit.x], EXIT_CHAR, WALKABLE | LEFT_EXIT);
        }
    }

    // Right exit
    if (CREATE_SCENE_X(map->unique) != SCENE_MAX_X - 1)
    {
        Coord exit = map_create_exit(map, RIGHT_EXIT);
        for (int8_t i = -2; i <= 2; ++i)
        {
            point_set(map->map[exit.y+i][exit.x], EXIT_CHAR, WALKABLE | RIGHT_EXIT);
        }
    }

    // Upper exit
    if (CREATE_SCENE_Y(map->unique) != 0)
    {
        Coord exit = map_create_exit(map, UPPER_EXIT);
        for (int8_t i = -2; i <= 2; ++i)
        {
            point_set(map->map[exit.y][exit.x+i], EXIT_CHAR, WALKABLE | UPPER_EXIT);
        }
    }

    // Lower exit
    if (CREATE_SCENE_Y(map->unique) != SCENE_MAX_Y - 1)
    {
        Coord exit = map_create_exit(map, LOWER_EXIT);
        for (int8_t i = -2; i <= 2; ++i)
        {
            point_set(map->map[exit.y][exit.x+i], EXIT_CHAR, WALKABLE | LOWER_EXIT);
        }
    }
}

bool
walkable(Map map, int _delta_x, int _delta_y)
{
    if (! IS_WALKABLE(map->map[HEAD_Y+_delta_y][HEAD_X+_delta_x]))
        return 0;
    else if (! IS_WALKABLE(map->map[LEFT_FOOT_Y+_delta_y][LEFT_FOOT_X+_delta_x]))
        return 0;
    else if (! IS_WALKABLE(map->map[RIGHT_FOOT_Y+_delta_y][RIGHT_FOOT_X+_delta_x]))
        return 0;
    else if (! IS_WALKABLE(map->map[LEFT_ARM_Y+_delta_y][LEFT_ARM_X+_delta_x]))
        return 0;
    else if (! IS_WALKABLE(map->map[RIGHT_ARM_Y+_delta_y][RIGHT_ARM_X+_delta_x]))
        return 0;
    else
        return 1;
}

void
map_debug(Map map)
{
    for (int y = 0; y < MAP_MAX_Y; ++y)
    {
        for (int x = 0; x < MAP_MAX_X; ++x)
            logger_dump("%c", point_get_content(map->map[y][x]));
        logger_dump("\n");
    }
}
