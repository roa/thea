#include <scenemanager.h>

SceneManager
scenemanager_init()
{
    SceneManager sm = calloc(sizeof(*sm), 1);
    sm->win = initscr();
    nodelay(stdscr, true);
    clear();
    noecho();
    cbreak();
    keypad(sm->win, TRUE);
    sm->current_map = map_init(sm->scene_x, sm->scene_y);
    sm->player = player_init(sm->player);
    player_set_rel(sm->player, 1, 1);
    refresh_screen(sm, 0, 0);
    sm->moved = false;
    return sm;
}

void
scenemanager_run(SceneManager sm)
{
    while (true)
    {
        uint64_t start = get_micro_time();
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
            default:
                refresh_screen(sm, 0, 0);
        }
        uint64_t diff = get_micro_time() - start;
        if (diff < 16666)
            usleep(16666 - diff);

        INCREMENT_FRAME(sm);
    }
}

void
scenemanager_free(SceneManager sm)
{
    endwin();
    map_free(sm->current_map);
    player_free(sm->player);
    free(sm);
    sm = NULL;
}

void
refresh_screen(SceneManager sm, int delta_x, int delta_y)
{
    int32_t player_x, player_y;
    player_get_rel(sm->player, &player_x, &player_y);
    if (walkable(sm->current_map, X_REL(delta_x, player_x), Y_REL(delta_y, player_y)))
    {
        player_x += delta_x;
        player_y += delta_y;
        player_move(sm->player, player_x, player_y);
        if (delta_x != 0 || delta_y != 0)
            sm->moved = true;
    }

    check_for_exit(sm);

    for (int y = 0; y < LINES; ++y)
        for (int x = 0; x < COLS; ++x)
            mvprintw(y, x, "%c", point_get_content(sm->current_map->map[y + player_y][x + player_x]));

    player_animate(sm->player, sm->frame, sm->moved);
    move(0, 0);
    refresh();
}

void
check_for_exit(SceneManager sm)
{
    Map map = sm->current_map;
    int32_t player_x, player_y;
    player_get_abs(sm->player, &player_x, &player_y);
    if (IS_EXIT(map->map[player_y][player_x])) {
        Map  new_map = NULL;
        Exit exit    = exit_list_search(map->exits, player_x, player_y),
             entry   = NULL;

        int32_t offset_x = 0,
                offset_y = 0;

        // TODO handle error
        if (exit == NULL) return;

        switch (exit->type)
        {
            case UPPER_EXIT:
                sm->scene_y -= 1;
                new_map = map_init(sm->scene_x, sm->scene_y);
                entry = exit_list_search_by_type(new_map->exits, LOWER_EXIT);
                offset_y = -5;
                break;
            case RIGHT_EXIT:
                sm->scene_x += 1;
                new_map = map_init(sm->scene_x, sm->scene_y);
                entry = exit_list_search_by_type(new_map->exits, LEFT_EXIT);
                offset_x = +5;
                break;
            case LOWER_EXIT:
                sm->scene_y += 1;
                new_map = map_init(sm->scene_x, sm->scene_y);
                entry = exit_list_search_by_type(new_map->exits, UPPER_EXIT);
                offset_y = +5;
                break;
            case LEFT_EXIT:
                sm->scene_x -= 1;
                new_map = map_init(sm->scene_x, sm->scene_y);
                entry = exit_list_search_by_type(new_map->exits, RIGHT_EXIT);
                offset_x = -5;
                break;
        }
        map_free(sm->current_map);
        sm->current_map = new_map;
        player_set_abs(sm->player, entry->x + offset_x, entry->y + offset_y);
    }
}

uint64_t
get_micro_time()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(uint64_t)1000000+tv.tv_usec;
}
