#include <map_exit.h>

Exit
exit_init(uint32_t x, uint32_t y, uint32_t type)
{
    Exit exit = calloc(sizeof(*exit), 1);
    exit->x    = x;
    exit->y    = y;
    exit->type = type;
    return exit;
}

void
exit_free(Exit exit)
{
    free(exit);
    exit = NULL;
}

ExitList
exit_list_init(uint32_t size)
{
    ExitList list = calloc(sizeof(*list), 1);
    list->size = size;
    list->used = 0;
    list->exits = calloc(sizeof(*(list->exits)), list->size);
    return list;
}

void
exit_list_free(ExitList list)
{
    for (int i = 0; i < list->used; ++i)
        exit_free(list->exits[i]);
    free(list->exits);
    list->exits = NULL;
    free(list);
    list = NULL;
}

void
exit_list_add(ExitList list, Exit exit)
{
    if (list->used == list->size)
    {
        list->size *= 2;
        Exit *new_exits = calloc(sizeof(*(list->exits)), list->size);
        for (int i = 0; i < list->used; ++i)
            new_exits[i] = list->exits[i];
        free(list->exits);
        list->exits = new_exits;
    }
    list->exits[list->used] = exit;
    list->used += 1;
}

Exit
exit_list_search(ExitList list, uint32_t x, uint32_t y)
{
    for (int i = 0; i < list->used; ++i)
        if (list->exits[i]->x == x && list->exits[i]->y == y)
            return list->exits[i];
    return NULL;
}

Exit
exit_list_search_by_type(ExitList list, uint32_t type)
{
    for (int i = 0; i < list->used; ++i)
        if (list->exits[i]->type == type)
            return list->exits[i];
    return NULL;
}
