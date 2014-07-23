#ifndef THEA_CONFIG_H
#define THEA_CONFIG_H

#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

struct Config
{
    const char *log_dir;
};

typedef struct Config Config;

void config_init(const char *fname);
const char *get_logdir();

#endif // THEA_CONFIG_H
