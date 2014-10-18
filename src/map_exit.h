#ifndef THEA_MAP_EXIT_H
#define THEA_MAP_EXIT_H

#include <stdint.h>
#include <stdlib.h>

#define UPPER_EXIT 0
#define RIGHT_EXIT 1
#define LOWER_EXIT 2
#define LEFT_EXIT  3

typedef struct ExitData
{
    uint32_t x;
    uint32_t y;
    uint32_t type;
} ExitData;

typedef ExitData *Exit;

Exit exit_init(uint32_t x, uint32_t y, uint32_t type);

void exit_free(Exit exit);

typedef struct ExitListData
{
    Exit     *exits;
    uint32_t  size;
    uint32_t  used;
} ExitListData;

typedef ExitListData *ExitList;

ExitList exit_list_init(uint32_t size);

void exit_list_free(ExitList list);

void exit_list_add(ExitList list, Exit exit);

Exit exit_list_search(ExitList list, uint32_t x, uint32_t y);

Exit exit_list_search_by_type(ExitList list, uint32_t type);

#endif // THEA_MAP_EXIT_H
