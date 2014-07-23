#include <scenemanager.h>

void
scenemanager_init(SceneManager *sm)
{
    sm->current_map = NULL;
    sm->win = initscr();
    timeout(-1);
    nodelay(stdscr, FALSE);
    clear();
    noecho();
    cbreak();
    keypad(sm->win, TRUE);
    sm->scene_x = 0;
    sm->scene_y = 0;
    map_init(&(sm->current_map), COLS, LINES, sm->scene_x, sm->scene_y);
    move_player_to(sm, 0, 0);
    refresh_screen(sm, 0, 0);
}

static WINDOW *test_win = NULL;
void test1() {
    test_win = newwin(10, 10, 10, 10);
    mvwprintw(test_win, 0, 0, "moep");
    wrefresh(test_win);
}

void test2() {
    delwin(test_win);
}

void
scenemanager_run(SceneManager *sm)
{
    while (true)
    {
        switch (getch())
        {
            case 'q':
                return;
            case KEY_RIGHT:
                refresh_screen(sm, +1, 0);
                break;
            case KEY_LEFT:
                refresh_screen(sm, -1, 0);
                break;
            case KEY_UP:
                refresh_screen(sm, 0, -1);
                break;
            case KEY_DOWN:
                refresh_screen(sm, 0, +1);
                break;
            case 'a':
                test1();
                break;
            case 'y':
                test2();
                break;
        }
    }
}

void
scenemanager_free(SceneManager *sm)
{
    endwin();
    map_free(&(sm->current_map));
}

void
refresh_screen(SceneManager *sm, int delta_x, int delta_y)
{
    if (walkable(sm->current_map, X_REL(delta_x, sm->x), Y_REL(delta_y, sm->y))) {
        sm->x += delta_x;
        sm->y += delta_y;
    }

    check_for_exit(sm);

    for (int y = 0; y < LINES; ++y)
        for (int x = 0; x < COLS; ++x)
            mvprintw(y, x, "%c", sm->current_map->map[Y_REL(y, sm->y)][X_REL(x, sm->x)]);

    PRINT_PLAYER();
    refresh();
}

bool
walkable(Map *map, int _delta_x, int _delta_y)
{
    if (! map->walkable[HEAD_Y+_delta_y][HEAD_X+_delta_x])
        return 0;
    else if (! map->walkable[LEFT_FOOT_Y+_delta_y][LEFT_FOOT_X+_delta_x])
        return 0;
    else if (! map->walkable[RIGHT_FOOT_Y+_delta_y][RIGHT_FOOT_X+_delta_x])
        return 0;
    else if (! map->walkable[LEFT_ARM_Y+_delta_y][LEFT_ARM_X+_delta_x])
        return 0;
    else if (! map->walkable[RIGHT_ARM_Y+_delta_y][RIGHT_ARM_X+_delta_x])
        return 0;
    else
        return 1;
}

void
check_for_exit(SceneManager *sm)
{
    Map *map = sm->current_map;
    if (map->exits[0] && TOUCH_EXIT_1(sm)) {
        sm->scene_x -= 1;
        map_free(&(sm->current_map));
        map_init(&(sm->current_map), COLS, LINES, sm->scene_x, sm->scene_y);
        move_player_to(sm, MAP_MAX_X - 5, sm->current_map->exits[1]);
        return;
    }
    if (map->exits[1] && TOUCH_EXIT_2(sm)) {
        sm->scene_x += 1;
        map_free(&(sm->current_map));
        map_init(&(sm->current_map), COLS, LINES, sm->scene_x, sm->scene_y);
        move_player_to(sm, 0, sm->current_map->exits[0]);
        return;
    }
    if (map->exits[2] && TOUCH_EXIT_3(sm)) {
        sm->scene_y -= 1;
        map_free(&(sm->current_map));
        map_init(&(sm->current_map), COLS, LINES, sm->scene_x, sm->scene_y);
        move_player_to(sm, sm->current_map->exits[3], MAP_MAX_Y - 5);
        return;
    }
    if (map->exits[3] && TOUCH_EXIT_4(sm)) {
        sm->scene_y += 1;
        map_free(&(sm->current_map));
        map_init(&(sm->current_map), COLS, LINES, sm->scene_x, sm->scene_y);
        move_player_to(sm, sm->current_map->exits[2], 0);
        return;
    }
}

void
move_player_to(SceneManager *sm, int x, int y)
{
    sm->x = x + COLS / 2 + 1 - SCREEN_X;
    sm->y = y + LINES / 2 - SCREEN_Y;
}
