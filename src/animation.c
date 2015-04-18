#include "animation.h"

Animation
animation_init(ObjectList objlist)
{
    Animation animation = calloc(sizeof(*animation), 1);

    animation->frame = 0;
    animation->objlist = objlist;
    return animation;
}

void
animation_free(Animation animation)
{
    object_list_free(animation->objlist);
    animation->objlist = NULL;
    free(animation);
    animation = NULL;
}

bool
animation_animate(Animation animation, int frame, bool moved)
{
    bool finished = false;
    ObjectList local_list = animation->objlist;
    Coord  coord = local_list->coord;
    Object obj = object_init(local_list->objects[local_list->slide]);
    for (int j = 0; j < obj->size; ++j)
    {
        if (point_get_content(obj->object[j]) == '\n')
        {
            coord.y += 1;
            coord.x  = local_list->coord.x;
            continue;
        }
        mvprintw(coord.y, coord.x, "%c", point_get_content(obj->object[j]));
        coord.x += 1;
    }

    if ( moved && local_list->slide < local_list->used - 1)
        animation->objlist->slide += 1;
    else
    {
        animation->objlist->slide = 0;
        finished = true;
    }

    object_free(obj);

    return finished;
}

bool
animation_move_towards(Animation animation, Coord coord)
{
    int32_t delta_x  = coord.x - animation->objlist->coord.x,
            toggle_x = 1;
    int32_t delta_y  = coord.y - animation->objlist->coord.y,
            toggle_y = 1;

    if (delta_x < 0)
        toggle_x = -1;

    if (delta_y < 0)
        toggle_y = -1;

    if (delta_x == 0 && delta_y == 0)
        return false;

    if (delta_x * toggle_x > delta_y * toggle_y * get_screen_ratio())
        animation->objlist->coord.x += (1 * toggle_x);
    else
        animation->objlist->coord.y += (1 * toggle_y);
    return true;
}
