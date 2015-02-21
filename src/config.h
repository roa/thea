#ifndef THEA_CONFIG_H
#define THEA_CONFIG_H

#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef struct Config
{
    const char *log_dir;
} Config;

void config_init(const char *fname);

const char *get_logdir();

#endif // THEA_CONFIG_H
