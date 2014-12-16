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
map_init(uint64_t scene_x, uint64_t scene_y)
{
    Map map = calloc(sizeof(*map), 1);

    //TODO: create from unique
    map->scene_x = scene_x;
    map->scene_y = scene_y;
    CREATE_UNIQUE(map->unique, scene_x, scene_y);
    logger_log("creating map [%llu][%llu] with unique identifier %llu", scene_x, scene_y, map->unique);
    srandom(map->unique);

    map->map = calloc(sizeof(Point *), MAP_MAX_Y);

    for (int y = 0; y < MAP_MAX_Y; ++y)
        map->map[y] = calloc(sizeof(Point), MAP_MAX_X);

    for (int y = 0; y < MAP_MAX_Y; ++y)
        for (int x = 0; x < MAP_MAX_X; ++x)
        {
            map->map[y][x] = point_init();
            if (x == MAP_X_REL_ZERO || x == MAP_X_REL_MAX)
                point_set(map->map[y][x], '?', 0);
            else if (y == MAP_Y_REL_ZERO || y == MAP_Y_REL_MAX)
                point_set(map->map[y][x], '?', 0);
            else
                point_set(map->map[y][x], ' ', WALKABLE);
        }

    map->exits = exit_list_init(4);
    map_add_exits(map);
    map_set_type(map);
    map_add_landscape(map);
    return map;
}

void map_set_type(Map map)
{
    if (map == NULL)
    {
        logger_log("used uninitalized map in %s", __FUNCTION__);
        return;
    }

    if (map->unique == 0)
        map->type = 0;
    else
        map->type = DICE(NUM_MAP_TYPES);
    logger_log("used map type %d in %s", map->type, __FUNCTION__);
}

void
map_add_landscape(Map map)
{
    switch(map->type)
    {
        case town:
            logger_log("town");
            map_try_add(map, "txt/town", 4);
            map_try_add(map, "txt/flora/tree", 100);
            break;
        case not_town:
            logger_log("not_town");
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
                {
                    added++;
                }
                free(obj_name);
            }
        }
}

void
map_free(Map map)
{
    exit_list_free(map->exits);
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

    for (int i = 0; i < obj->size; ++i)
    {
        if (point_get_content(obj->object[i]) == '\n')
        {
            ++local_y;
            local_x = p->x;
            continue;
        }
        if (point_get_content(obj->object[i]) != ' ')
            point_set(map->map[local_y][local_x++], point_get_content(obj->object[i]), 0);
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

void
map_add_exits(Map map)
{
    // Left exit
    if (map->scene_x != 0)
    {
        uint32_t x = MAP_X_REL_ZERO;
        uint32_t y = random() % MAP_HEIGHT + MAP_Y_REL_ZERO;
        for (int8_t i = -2; i <= 2; ++i)
        {
            Exit exit = exit_init(x, y+i, LEFT_EXIT);
            point_set(map->map[y+i][x], EXIT_CHAR, WALKABLE | EXIT);
            exit_list_add(map->exits, exit);
        }
    }

    // Right exit
    if (map->scene_x != SCENE_MAX_X - 1)
    {
        uint32_t x = MAP_X_REL_MAX;
        uint32_t y = random() % MAP_HEIGHT + MAP_Y_REL_ZERO;
        for (int8_t i = -2; i <= 2; ++i)
        {
            Exit exit = exit_init(x, y+i, RIGHT_EXIT);
            point_set(map->map[y+i][x], EXIT_CHAR, WALKABLE | EXIT);
            exit_list_add(map->exits, exit);
        }
    }

    // Upper exit
    if (map->scene_y != 0)
    {
        uint32_t x = random() % MAP_WIDTH + MAP_X_REL_ZERO;
        uint32_t y = MAP_Y_REL_ZERO;
        for (int8_t i = -2; i <= 2; ++i)
        {
            Exit exit = exit_init(x+i, y, UPPER_EXIT);
            point_set(map->map[y][x+i], EXIT_CHAR, WALKABLE | EXIT);
            exit_list_add(map->exits, exit);

        }
    }

    // Lower exit
    if (map->scene_y != SCENE_MAX_Y - 1)
    {
        uint32_t x = random() % MAP_WIDTH + MAP_X_REL_ZERO;
        uint32_t y = MAP_Y_REL_MAX;
        for (int8_t i = -2; i <= 2; ++i)
        {
            Exit exit = exit_init(x+i, y, LOWER_EXIT);
            point_set(map->map[y][x+i], EXIT_CHAR, WALKABLE | EXIT);
            exit_list_add(map->exits, exit);

        }
    }
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
