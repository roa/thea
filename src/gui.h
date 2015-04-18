#ifndef THEA_GUI_H
#define THEA_GUI_H

#include <helper.h>
#include <ncurses.h>
#include <menu.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct TitleData
{
    char *label;
    void (* func)(void *);
} TitleData;

typedef TitleData *Title;

Title title_init(char *label, void (* func)());

typedef struct GuiMenuData
{
    WINDOW    *menu_win;
    MENU      *menu;
    ITEM     **it;
    Title     *titles;
    uint32_t   size;
    bool       posted;
    struct GuiMenuData *parent;
    struct GuiMenuData *child;
} GuiMenuData;

typedef GuiMenuData *GuiMenu;

GuiMenu gui_menu_init(Title *titles, uint32_t size, uint32_t x, uint32_t y);

void gui_menu_post(GuiMenu gui_menu, bool recursive);

void gui_menu_unpost(GuiMenu gui_menu, bool recursive);

bool gui_menu_posted(GuiMenu gui_menu);

void gui_menu_refresh(GuiMenu gui_menu);

void gui_menu_add_child(GuiMenu gui_menu, GuiMenu child);

void gui_menu_del_child(GuiMenu gui_menu);

void gui_menu_free(GuiMenu *gui_menu);

#endif // THEA_GUI_H
