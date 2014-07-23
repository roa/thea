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
 *    |<--------->|      MAP_MAX_X
 *                |-->   value of x_rel_zero
 * <----------------->   map->max_x
 *
 * same for y values
 * y = 0 is on the top
 *
 ***********************************************/

void
map_init(Map **map, int screen_width, int screen_height, int64_t scene_x, int64_t scene_y)
{
    if ((*map) == NULL)
        (*map) = calloc(sizeof(*(*map)), 1);

    CREATE_UNIQUE((*map)->unique, scene_x, scene_y);
    logger_log("unique %lu", (*map)->unique);
    srandom((*map)->unique);

    (*map)->max_x = MAP_MAX_X + 2 * screen_width;
    (*map)->max_y = MAP_MAX_Y + 2 * screen_height;
    (*map)->x_rel_zero = screen_width - 1;
    (*map)->y_rel_zero = screen_height - 1;
    (*map)->x_rel_max = screen_width + MAP_MAX_X - 1;
    (*map)->y_rel_max = screen_height + MAP_MAX_Y - 1;

    (*map)->map      = malloc((*map)->max_y * sizeof(char *));
    (*map)->walkable = malloc((*map)->max_y * sizeof(bool *));

    for (int y = 0; y < (*map)->max_y; ++y)
    {
        (*map)->map[y]      = malloc((*map)->max_x * sizeof(char));
        (*map)->walkable[y] = malloc((*map)->max_x * sizeof(bool));
    }

    for (int y = 0; y < (*map)->max_y; ++y)
        for (int x = 0; x < (*map)->max_x; ++x)
        {
            if (x == (*map)->x_rel_zero || x == (*map)->x_rel_max)
            {
                (*map)->map[y][x] = '?';
                (*map)->walkable[y][x] = false;
            }
            else if (y == (*map)->y_rel_zero || y == (*map)->y_rel_max)
            {
                (*map)->map[y][x] = '?';
                (*map)->walkable[y][x] = false;
            }
            else
            {
                (*map)->map[y][x] = ' ';
                (*map)->walkable[y][x] = true;
            }
        }

    SET_EXITS((*map), scene_x, scene_y);
    map_add_houses(map);
}

void
map_free(Map **map)
{
    for (int y = 0; y < (*map)->max_y; ++y)
    {
        free((*map)->map[y]);
        (*map)->map[y] = NULL;
        free((*map)->walkable[y]);
        (*map)->walkable[y] = NULL;
    }
    free((*map)->map);
    free((*map)->walkable);
    (*map)->map = NULL;
    map = NULL;
}
void map_add_houses(Map **map)
{
    /* TODO randomize */
    int num_house = 0;
    int tries = 0;
    Point p = {MAP_X_REL(1), MAP_Y_REL(1)};
    while (num_house < 7 || tries == 3)
    {
        for (int local_y = p.y; local_y< MAP_MAX_Y + (*map)->y_rel_zero; ++local_y)
        {
            MAYBE_CONTINUE(95);
            for (int local_x = p.x; local_x < MAP_MAX_X + (*map)->x_rel_zero; ++local_x )
            {
                MAYBE_CONTINUE(95);
                Point local_p = {local_x, local_y};
                if (map_add_object(map, "txt/house.txt", &local_p))
                {
                    num_house++;
                    logger_log("house %d %d", local_p.x, local_p.y);
                }
            }
        }
        ++tries;
    }
}

bool
map_add_object(Map **map, const char *obj_name, Point *p)
{
    int local_x = p->x;
    int local_y = p->y;

    Object obj;
    object_init(&obj, obj_name);

    if (!map_object_fits(map, &obj, p))
        return false;

    for (int i = 0; i < obj.size; ++i)
    {
        if (obj.object[i] == '\n')
        {
            ++local_y;
            local_x = p->x;
            continue;
        }
        if (obj.object[i] != ' ')
            (*map)->walkable[local_y][local_x] = false;
        (*map)->map[local_y][local_x++] = obj.object[i];
    }

    object_free(&obj);
    return true;
}

bool
map_object_fits(Map **map, Object *obj, Point *p)
{
    /* TODO use borders */
    /*
    if (((p->x + obj->width) > MAP_MAX_X) || ((p->x - safe_space) < 0))
        return false;

    if (((p->y + obj->height) > MAP_MAX_Y) || ((p->y - safe_space) < 0))
        return false;
    */
    for (int y = p->y - SAFE_SPACE; y < p->y + obj->height + SAFE_SPACE; ++y)
        for (int x = p->x - SAFE_SPACE; x < p->x + obj->width + SAFE_SPACE; ++x)
            if ((*map)->map[y][x] != ' ')
                return false;
    return true;
}

void
map_debug(Map *map)
{
    for (int y = 0; y < map->max_y; ++y)
    {
        for (int x = 0; x < map->max_x; ++x)
            logger_dump("%c", map->map[y][x]);
        logger_dump("\n");
    }
}
