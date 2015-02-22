#ifndef THEA_POINT_H
#define THEA_POINT_H

#include <stdint.h>
#include <stdlib.h>

#define WALKABLE   0b0000001
#define UPPER_EXIT 0b0000010
#define RIGHT_EXIT 0b0000100
#define LOWER_EXIT 0b0001000
#define LEFT_EXIT  0b0010000
#define HOUSE_EXIT 0b0100000
#define MAP_EXIT   0b1000000

#define IS_WALKABLE(_point) \
    ((point_get_type(_point) & WALKABLE) > 0)

#define IS_UPPER_EXIT(_point) \
    ((point_get_type(_point) & UPPER_EXIT) > 0)

#define IS_RIGHT_EXIT(_point) \
    ((point_get_type(_point) & RIGHT_EXIT) > 0)

#define IS_LOWER_EXIT(_point) \
    ((point_get_type(_point) & LOWER_EXIT) > 0)

#define IS_LEFT_EXIT(_point) \
    ((point_get_type(_point) & LEFT_EXIT ) > 0)

#define IS_HOUSE_EXIT(_point) \
    ((point_get_type(_point) & HOUSE_EXIT ) > 0)

#define IS_MAP_EXIT(_point) \
    ((point_get_type(_point) & MAP_EXIT ) > 0)

#define IS_EXIT(_point) \
    IS_UPPER_EXIT(_point) || \
    IS_LOWER_EXIT(_point) || \
    IS_RIGHT_EXIT(_point) || \
    IS_LEFT_EXIT(_point)  || \
    IS_HOUSE_EXIT(_point) || \
    IS_MAP_EXIT(_point)

typedef struct PointData
{
    char     content;
    uint32_t type;
} PointData;

typedef PointData *Point;

Point point_init();

void point_free(Point point);

void point_set(Point point, char content, int32_t type);

char point_get_content(Point point);

int32_t point_get_type(Point point);

#endif // THEA_POINT_H
