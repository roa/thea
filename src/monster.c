#include "monster.h"

Monster
monster_init(const char *name, uint32_t x, uint32_t y)
{
    Monster monster = calloc(sizeof(*monster), 1);

    ObjectList objlist  = list_from_dir(name);
    objlist->coord = (Coord) {x, y};
    monster->animation = animation_init(objlist);
    monster->stats = stats_init();
    return monster;
}

void
monster_free(Monster *monster)
{
    animation_free((*monster)->animation);
    stats_free(&(*monster)->stats);
    free(*monster);
    *monster = NULL;
}

void
monster_move(Monster monster, int32_t x, int32_t y)
{
    monster->x = x;
    monster->y = y;
    monster->animation->objlist->coord.x = x;
    monster->animation->objlist->coord.y = y;
}

bool
monster_animate(Monster monster, int32_t frame, bool moved)
{
    if (stats_is_dead(monster->stats))
        return false;
    animation_animate(monster->animation, frame, moved);
    return true;
}

MonsterGroup
monster_group_init(uint8_t size)
{
    MonsterGroup group = calloc(sizeof(*group), 1);
    
    group->size = size;
    group->monsters = calloc(sizeof(*group->monsters), group->size);

    for (uint8_t i = 0; i < group->size; ++i)
    {
        Coord pos = monster_group_pos(group, i);
        group->monsters[i] = monster_init("txt/animations/person", pos.x, pos.y);
    }

    return group;
}

void
monster_group_free(MonsterGroup *group)
{
    for (uint8_t i = 0; i < (*group)->size; ++i)
    {
        monster_free(&(*group)->monsters[i]);
        (*group)->monsters[i] = NULL;
    }
    free((*group)->monsters);
    free(*group);
}

Monster
monster_group_get(MonsterGroup group, uint8_t num)
{
    //TODO maybe don't start at 0?
    if (num >= group->size)
        num = 0;
    return group->monsters[num];
}

Coord
monster_group_pos(MonsterGroup group, int8_t num)
{
    int8_t toggle = -1;
    
    for (int8_t i = 0; i <= num; ++i)
        toggle *= -1;

    double lines = (double) LINES;
    double cols  = (double) COLS;

    uint64_t x = (uint64_t) (cols / 2 + 10 +4* num * toggle);
    uint64_t y = (uint64_t) (-lines/cols * x + LINES + 10);

    return (Coord) {.x = x, .y = y};
}
