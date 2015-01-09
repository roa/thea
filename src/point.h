#ifndef THEA_POINT_H
#define THEA_POINT_H

#include <stdint.h>
#include <stdlib.h>

#define WALKABLE 0b0001
#define EXIT     0b0010

#define IS_WALKABLE(_point) ((point_get_type(_point) & WALKABLE) > 0)
#define IS_EXIT(_point) ((point_get_type(_point) & EXIT) > 0)

typedef struct PointData
{
    char    content;
    int32_t type;
} PointData;

typedef PointData *Point;

Point point_init();

void point_free(Point point);

void point_set(Point point, char content, int32_t type);

char point_get_content(Point point);

int32_t point_get_type(Point point);

#endif // THEA_POINT_H
