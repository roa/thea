#include <stdio.h>
#include <ncurses.h>
#include <locale.h>

#include <logger.h>
#include <scenemanager.h>
#include <object.h>

int
main()
{
    setlocale(LC_ALL, "");
    config_init("config/thea.lua");
    logger_init();

    SceneManager sm = scenemanager_init();
    scenemanager_run(sm);
    scenemanager_free(sm);

    return 0;
}
