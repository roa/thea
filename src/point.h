#ifndef THEA_POINT_H
#define THEA_POINT_H

#include <stdint.h>
#include <stdlib.h>

#define WALKABLE   0b00001
#define UPPER_EXIT 0b00010
#define RIGHT_EXIT 0b00100
#define LOWER_EXIT 0b01000
#define LEFT_EXIT  0b10000

#define IS_WALKABLE(_point) ((point_get_type(_point) & WALKABLE) > 0)

#define IS_UPPER_EXIT(_point) \
    ((point_get_type(_point) & UPPER_EXIT) > 0)

#define IS_RIGHT_EXIT(_point) \
    ((point_get_type(_point) & RIGHT_EXIT) > 0)

#define IS_LOWER_EXIT(_point) \
    ((point_get_type(_point) & LOWER_EXIT) > 0)

#define IS_LEFT_EXIT(_point) \
    ((point_get_type(_point) & LEFT_EXIT ) > 0)

#define IS_EXIT(_point) \
    IS_UPPER_EXIT(_point) || IS_LOWER_EXIT(_point) || IS_RIGHT_EXIT(_point) || IS_LEFT_EXIT(_point)

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
