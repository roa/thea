#include <stdio.h>
#include <ncurses.h>
#include <logger.h>
#include <scenemanager.h>

#include <object.h>

int
main()
{
    config_init("config/thea.lua");
    logger_init();
    dir_globbing();
    SceneManager sm;
    scenemanager_init(&sm);
    scenemanager_run(&sm);
    scenemanager_free(&sm);
    return 0;
}
