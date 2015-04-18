#include <gui.h>

Title
title_init(char *label, void(*func)(void *))
{
    Title title = calloc(1, sizeof(*title));
    title->label = label;
    title->func  = func;
    return title;
}

GuiMenu
gui_menu_init(Title *titles, uint32_t size, uint32_t x, uint32_t y)
{
    GuiMenu gui_menu = calloc(1, sizeof(*gui_menu));

    gui_menu->titles = titles;
    gui_menu->size = size;
    gui_menu->it = calloc(size + 1, sizeof(*gui_menu->it));
    for (uint32_t i = 0; i < size; ++i)
    {   
        gui_menu->it[i] = new_item(titles[i]->label, NULL);
    }   
    gui_menu->it[size] = NULL;
    gui_menu->menu = new_menu(gui_menu->it);
    gui_menu->menu_win = newwin(10, 10, y, x); 
    set_menu_win(gui_menu->menu, gui_menu->menu_win);
    set_menu_mark(gui_menu->menu, "->");

    return gui_menu;
}

void
gui_menu_post(GuiMenu gui_menu, bool recursive)
{
    if (recursive && gui_menu->parent != NULL)
        gui_menu_post(gui_menu->parent, true);
    post_menu(gui_menu->menu);
    gui_menu->posted = true;
}

void
gui_menu_unpost(GuiMenu gui_menu, bool recursive)
{
    if (recursive && gui_menu->parent != NULL)
        gui_menu_unpost(gui_menu->parent, true);
    unpost_menu(gui_menu->menu);
    gui_menu->posted = false;
}

bool
gui_menu_posted(GuiMenu gui_menu)
{
    return gui_menu->posted;
}

void
gui_menu_refresh(GuiMenu gui_menu)
{
    if (gui_menu->parent != NULL)
        gui_menu_refresh(gui_menu->parent);
    wrefresh(gui_menu->menu_win);
}

void
gui_menu_add_child(GuiMenu gui_menu, GuiMenu child)
{
    gui_menu->child = child;
    child->parent = gui_menu;
}

void
gui_menu_del_child(GuiMenu gui_menu)
{
    if (gui_menu->child == NULL)
        return;
    if (gui_menu_posted(gui_menu->child))
        gui_menu_unpost(gui_menu->child, true);
    gui_menu_refresh(gui_menu->child);
    gui_menu_del_child(gui_menu->child);
    gui_menu_free(&(gui_menu->child));
    gui_menu->child = NULL;
}

void
gui_menu_free(GuiMenu *gui_menu)
{
    for (uint32_t i = 0; i < (*gui_menu)->size; ++i)
    {   
        free_item((*gui_menu)->it[i]);
        free((*gui_menu)->titles[i]->label);
        free((*gui_menu)->titles[i]);
    }   
    free_menu((*gui_menu)->menu);
    free((*gui_menu)->it);
    free((*gui_menu)->titles);
    delwin((*gui_menu)->menu_win);
    free(*gui_menu);
}
