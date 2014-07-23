#ifndef THEA_OBJECT_H
#define THEA_OBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <logger.h>

struct Object
{
    char   *object;
    size_t  size;
    size_t  width;
    size_t  height;
};

typedef struct Object Object;

void object_init(Object *obj, const char *obj_name);
void object_free(Object *obj);
size_t object_get_length(FILE *fp);
void dir_globbing();

#endif //THEA_OBJECT_H
