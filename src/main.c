#include <stdio.h>
#include <ncurses.h>
#include <locale.h>

#include <logger.h>
#include <scenemanager.h>
#include <battlemanager.h>

int
main()
{
    setlocale(LC_ALL, "");
    config_init("config/thea.lua");
    logger_init();

    WINDOW *win = initscr();
    nodelay(win, true);
    clear();
    noecho();
    cbreak();
    nl();
    curs_set(0);
    keypad(win, TRUE);
    SceneManager sm = scenemanager_init(win);
    scenemanager_run(sm);
    scenemanager_free(&sm);
    /*
    Player player = player_init();
    BattleManager bm = battlemanager_init(win, player);
    battlemanager_run(bm);
    battlemanager_free(&bm);
    */
    logger_deinit();
    endwin();
    return 0;
}
