#include <config.h>

static Config config;

void config_init(const char *fname)
{
    Config *c = &config;
    lua_State *L = luaL_newstate();
    if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
        printf("can not load config file: %s\n", lua_tostring(L, -1));

    lua_getglobal(L, "log_dir");
    if (!lua_isstring(L, 1))
        printf("can not load log_dir \n");
    else
        c->log_dir = lua_tostring(L, 1);
    lua_pop(L, 1);

    lua_close(L);
}

const char *get_logdir()
{
    Config *c = &config;
    return c->log_dir;
}
