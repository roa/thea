#ifndef THEA_OBJECT_H
#define THEA_OBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include <logger.h>
#include <helper.h>
#include <coord.h>
#include <point.h>

typedef struct ObjectListData
{
    char    **objects;
    int32_t   size;
    int32_t   used;
    int32_t   slide;
    Coord     coord;
} ObjectListData;

typedef ObjectListData *ObjectList;

ObjectList object_list_init(int32_t size);

void object_list_free(ObjectList objlist);

void object_list_add(ObjectList objlist, const char *obj);

typedef struct ObjectData
{
    Point  *object;
    size_t  size;
    size_t  width;
    size_t  height;
    size_t  used;
} ObjectData;

typedef ObjectData *Object;

Object object_init(const char *obj_name);

void object_free(Object obj);

size_t object_get_length(FILE *fp);

char* dir_globbing(const char *dirname);

ObjectList list_from_dir(const char *dname);

#endif //THEA_OBJECT_H
