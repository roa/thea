#include <battlemanager.h>

BattleManager
battlemanager_init(WINDOW *win, Player player)
{
    BattleManager bm = calloc(1, sizeof(*bm));
    bm->monster = monster_group_init(4);
    bm->win = win;
    bm->map = map_init(0, BATTLE);
    bm->player = player;
    bm->state = CALC_TURN;

    bm->current = battlemanager_main_menu(bm);
    battlemanager_clean_menu(bm);

    Coord player_coord = player_battle_pos();
    player_battle_move(bm->player, player_coord.x, player_coord.y);

    return bm;
}

void
battlemanager_run(BattleManager bm)
{
    while (true)
    {
        uint64_t start = get_micro_time();
        if (bm->quit)
            return;

        switch(getch())
        {
            case 'q':
                return;
            case KEY_DOWN:
                menu_driver(bm->current->menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(bm->current->menu, REQ_UP_ITEM);
                break;
            case KEY_LEFT:
                battlemanager_clean_menu(bm);
                break;
            case 0xA:
            case KEY_RIGHT:
                battlemanager_action(bm);
                break;
        }
        battlemanager_refresh(bm);
        uint64_t diff = get_micro_time() - start;
        if (diff < 16666)
            usleep(16666 - diff);
        INCREMENT_FRAME(bm);
    }
}

void
battlemanager_free(BattleManager *bm)
{
    gui_menu_free(&(*bm)->current);
    monster_group_free(&(*bm)->monster);
    map_free((*bm)->map);
    free(*bm);
    *bm = NULL;
}

void
battlemanager_refresh(BattleManager bm)
{
    for (int y = 0; y < LINES - MENU_OFFSET; ++y)
        for (int x = 0; x < COLS; ++x)
            mvwprintw(bm->win, y, x, "%c", point_get_content(bm->map->map[y][x]));

    mvwprintw(bm->win, 1, 1, "%s %lld", "test", get_micro_time());

    process_turn(bm, bm->frame, true);

    if (bm->state == IDLE)
        gui_menu_post(bm->current, true);
    else
        gui_menu_unpost(bm->current, true);
    wrefresh(bm->win);
    gui_menu_refresh(bm->current);
}

void
battlemanager_clean_menu(BattleManager bm)
{
    if (bm->current->parent != NULL)
        bm->current = bm->current->parent;
    gui_menu_del_child(bm->current);

    for (int y = LINES - 5; y < LINES; ++y)
        for (int x = 0; x < COLS; ++x)
            mvwprintw(bm->win, y, x, "%c", ' ');
    for (int i = 0; i < COLS; ++i)
        mvwprintw(bm->win, LINES - MENU_OFFSET, i, "=");
    
    gui_menu_unpost(bm->current, true);
    gui_menu_post(bm->current, true);
}

void
battlemanager_action(BattleManager bm)
{
    if (bm->state != IDLE)
        return;

    bm->index = item_index(current_item(bm->current->menu));
    if (bm->current->titles[bm->index]->func != NULL)
        bm->current->titles[bm->index]->func(bm);
}

void
battlemanager_set_state_move(BattleManager bm)
{
    bm->state = PLAYER_MOVE_1;
}

void
process_turn(BattleManager bm, uint32_t frame, bool moved)
{
    Monster monster = monster_group_get(bm->monster, bm->index);
    Coord player_coord = player_battle_pos();
    bool done = true;

    if (bm->frame % 5 != 0)
    {
        player_animate(bm->player, frame, moved);
        for (uint8_t i = 0; i < bm->monster->size; ++i)
        {
            monster_animate(bm->monster->monsters[i], frame, false);
        }
        return;
    }

    switch (bm->state)
    {
        case PLAYER_MOVE_1:
            player_animate(bm->player, frame, moved);
            if (!animation_move_towards(bm->player->animation, monster->animation->objlist->coord))
            {
                bm->state = PLAYER_MOVE_2;
                stats_attack_dmg(bm->player->stats, monster->stats);
            }
            break;
        case PLAYER_MOVE_2:
            player_animate(bm->player, frame, moved);
            if (!animation_move_towards(bm->player->animation, player_coord))
            {
                bm->state = CALC_TURN;
                bm->player->stats->charged_speed = 0;
                battlemanager_clean_menu(bm);
            }
            break;
        case CALC_TURN:
            battlemanager_calc_turn(bm);
            break;
        case MONSTER_TURN_1:
            for (uint8_t i = 0; i < bm->monster->size; ++i)
            {
                if (stats_is_turn(bm->monster->monsters[i]->stats))
                {
                    if (!animation_move_towards(
                                bm->monster->monsters[i]->animation,
                                player_coord))
                    {
                        bm->state = MONSTER_TURN_2;
                        stats_attack_dmg(monster->stats, bm->player->stats);
                    }
                    done = false;
                    break;
                }
            }
            if (done)
                bm->state = CALC_TURN;
            break;
        case MONSTER_TURN_2:
            for (uint8_t i = 0; i < bm->monster->size; ++i)
            {
                if (stats_is_turn(bm->monster->monsters[i]->stats))
                {
                    if (!animation_move_towards(
                                bm->monster->monsters[i]->animation,
                                monster_group_pos(bm->monster, i)))
                    {
                        bm->state = MONSTER_TURN_1;
                        bm->monster->monsters[i]->stats->charged_speed = 0;
                    }
                    break;
                }
            }
            break;
        default:
            player_animate(bm->player, frame, false);
    }

    bool alive = false;
    for (uint8_t i = 0; i < bm->monster->size; ++i)
    {
        if (monster_animate(bm->monster->monsters[i], frame, false))
            alive = true;
    }

    if (!alive || stats_is_dead(bm->player->stats))
        bm->quit = true;

    player_animate(bm->player, frame, false);
}

void
battlemanager_switch_to_attack(BattleManager bm)
{
    GuiMenu attack = battlemanager_attack_menu(bm);
    gui_menu_add_child(bm->current, attack);
    bm->current = attack;
}

void
battlemanager_calc_turn(BattleManager bm)
{
    for (;;)
    {
        bool last = false;
        stats_charge_speed(bm->player->stats);
        if (stats_is_turn(bm->player->stats))
        {
            bm->state = IDLE;
            break;
        }
        for (uint8_t i = 0; i < bm->monster->size; ++i)
        {
            stats_charge_speed(bm->monster->monsters[i]->stats);
            if (stats_is_turn(bm->monster->monsters[i]->stats))
            {
                bm->state = MONSTER_TURN_1;
                last = true;
            }
        }
        if (last)
            break;
    }
}

GuiMenu
battlemanager_main_menu(BattleManager bm)
{
    GuiMenu gui = calloc(1, sizeof(*gui));
    
    Title *titles = calloc(4, sizeof(*titles));
    titles[0] = title_init(create_dyn_string("Attack"), battlemanager_switch_to_attack);
    titles[1] = title_init(create_dyn_string("M2"), NULL);
    titles[2] = title_init(create_dyn_string("M3"), NULL);
    titles[3] = title_init(create_dyn_string("M4"), NULL);

    gui = gui_menu_init(titles, 4, 1, LINES - MENU_OFFSET + 1); 

    return gui;
}

GuiMenu
battlemanager_attack_menu(BattleManager bm)
{
    GuiMenu gui = calloc(1, sizeof(*gui));
    
    Title *titles = calloc(bm->monster->size, sizeof(*titles));
    for (uint8_t i = 0; i < bm->monster->size; ++i)
    {
        Monster monster = monster_group_get(bm->monster, i);
        if (stats_is_dead(monster->stats))
            titles[i] = title_init(create_dyn_string("[DEAD] Monster"), NULL);
        else
            titles[i] = title_init(create_num_string("Monster", i + 1), battlemanager_set_state_move);
    }

    gui = gui_menu_init(titles, bm->monster->size, 9, LINES - MENU_OFFSET + 1); 

    return gui;
}
