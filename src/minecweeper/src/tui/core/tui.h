#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

typedef enum {
    COLOR_PAIR_DEFAULT = 1,
    COLOR_PAIR_HIDDEN,
    COLOR_PAIR_REVEALED, 
    COLOR_PAIR_FLAG,
    COLOR_PAIR_MINE,
    COLOR_PAIR_CURSOR,
    COLOR_PAIR_MENU,
    COLOR_PAIR_MENU_SELECTED
} ColorPairs;

void init_tui();
void cleanup_tui();
void refresh_display();

#endif