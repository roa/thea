#include "object.h"

void
object_init(Object *obj, const char *obj_name)
{
    FILE *fp = fopen(obj_name, "r");
    if (fp == NULL)
        logger_log("%s", strerror(errno));

    obj->size   = object_get_length(fp);
    obj->width  = 0;
    obj->height = 0;
    obj->object = calloc(obj->size, sizeof *(obj->object));

    int width = 0;
    for (int i = 0; i < obj->size; ++i)
    {
        obj->object[i] = fgetc(fp);
        if (obj->object[i] == '\n')
        {
            if (obj->width < i)
                obj->width = width;
            obj->height += 1;
            width = 0;
        }
        else
            ++width;
    }
    fclose(fp);
}

void
object_free(Object *obj)
{
    free(obj->object);
    obj->object = NULL;
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

void
dir_globbing()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("txt")) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
            logger_log("%s", ent->d_name);
        closedir (dir);
    }
    else
    {
          logger_log("%s", strerror(errno));
    }
}
