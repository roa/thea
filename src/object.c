#include "object.h"

static int
cmpstringp(const void *p1, const void *p2)
{
    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

ObjectList
object_list_init(int32_t size)
{
    ObjectList objlist = calloc(sizeof(*objlist), 1);

    objlist->size  = size;
    objlist->used  = 0;
    objlist->slide = 0;
    objlist->coord = (Coord) { .x = 0, .y = 0 };
    objlist->objects = calloc(sizeof(char *), objlist->size);
    return objlist;
}

void
object_list_free(ObjectList objlist)
{
    for (int i = 0; i < objlist->used; ++i)
    {
        free(objlist->objects[i]);
        objlist->objects[i] = NULL;
    }
    free(objlist->objects);
    objlist->objects = NULL;
    free(objlist);
    objlist = NULL;
}

void
object_list_add(ObjectList objlist, const char *obj)
{
    if (objlist->used == objlist->size)
    {
        objlist->size = objlist->size * 2;
        char **swap = objlist->objects;
        objlist->objects = calloc(sizeof(char *), objlist->size);
        for (int i = 0; i < objlist->used; ++i)
            objlist->objects[i] = swap[i];
        free(swap);
    }
    size_t len = strlen(obj);
    objlist->objects[objlist->used] = calloc(sizeof(char), len + 1);
    memcpy(objlist->objects[objlist->used], obj, len);
    objlist->used = objlist->used + 1;
}

void
object_list_sort(ObjectList objlist)
{
    qsort(
        objlist->objects,
        objlist->used,
        sizeof(char *),
        cmpstringp
    );
}

Object
object_init(const char *obj_name)
{
    Object obj = calloc(sizeof(*obj), 1);
    FILE *fp = fopen(obj_name, "r");
    if (fp == NULL)
        logger_log("%s", strerror(errno));

    obj->size   = object_get_length(fp);
    obj->object = calloc(obj->size, sizeof *(obj->object));

    int width = 0;
    int type  = 0;

    for (int i = 0; i < obj->size; ++i)
    {
        char content = fgetc(fp);

        if (content == '')
        {
            type ^= WALKABLE;
            continue;
        }
        if (content == '\n')
        {
            if (obj->width < width)
                obj->width = width;
            obj->height += 1;
            width = 0;
        }
        else
            ++width;

        obj->object[obj->used] = point_init();
        point_set(obj->object[obj->used], content, type);
        obj->used += 1;
    }

    fclose(fp);
    return obj;
}

void
object_free(Object obj)
{
    for (int i = 0; i < obj->used; ++i)
        point_free(obj->object[i]);
    free(obj->object);
    obj->object = NULL;
    free(obj);
    obj = NULL;
}

size_t
object_get_length(FILE *fp)
{
    fpos_t pos;
    long start, stop;

    if ((start = ftell(fp)))
        logger_log("%s", strerror(errno));

    if (fgetpos(fp, &pos))
        logger_log("%s", strerror(errno));

    if (fseek(fp, 0, SEEK_END) || (stop = ftell(fp)) == -1)
        logger_log("%s", strerror(errno));

    fsetpos(fp, &pos);
    return stop - start;
}

char*
dir_globbing(const char *dirname)
{
    ObjectList objlist = list_from_dir(dirname);
    char *choosen = objlist->objects[DICE(objlist->used)];
    int result_len = strlen(choosen) + 1;
    char *result = calloc(result_len, sizeof *result);
    memcpy(result, choosen, result_len);
    object_list_free(objlist);
    return result;
}

ObjectList
list_from_dir(const char *dname)
{
    DIR           *dir;
    struct dirent *ent;
    ObjectList    objlist = object_list_init(2);
    int dlength = strlen(dname) + 2;
    if ((dir = opendir(dname)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
                continue;

            char *result = calloc(dlength + strlen(ent->d_name), sizeof(*result));
            sprintf(result, "%s/%s", dname, ent->d_name);
            object_list_add(objlist, result);
            free(result);
        }
        closedir(dir);
        object_list_sort(objlist);
    }
    else
        logger_log("%s", strerror(errno));
    return objlist;
}
