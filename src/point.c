#include <point.h>

Point
point_init()
{
    Point point = calloc(sizeof(*point), 1);
    point->content = ' ';
    return point;
}

void
point_free(Point point)
{
    free(point);
}

void
point_set(Point point, char content, int32_t type)
{
    point->content = content;
    point->type = type;
}

char
point_get_content(Point point)
{
    return point->content;
}

int32_t
point_get_type(Point point)
{
    return point->type;
}
