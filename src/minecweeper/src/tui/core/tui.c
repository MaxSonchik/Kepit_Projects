#include "tui.h"
#include <stdlib.h>
#include <locale.h>

void init_tui() {
    setlocale(LC_ALL, "");
    
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(100);
    
    if (has_colors()) {
        start_color();
        init_pair(COLOR_PAIR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_PAIR_HIDDEN, COLOR_WHITE, COLOR_BLUE);
        init_pair(COLOR_PAIR_REVEALED, COLOR_BLACK, COLOR_WHITE);
        init_pair(COLOR_PAIR_FLAG, COLOR_RED, COLOR_BLUE);
        init_pair(COLOR_PAIR_MINE, COLOR_RED, COLOR_WHITE);
        init_pair(COLOR_PAIR_CURSOR, COLOR_BLACK, COLOR_YELLOW);
        init_pair(COLOR_PAIR_MENU, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_PAIR_MENU_SELECTED, COLOR_BLACK, COLOR_WHITE);
    }
    

    clear();
}

void cleanup_tui() {
    endwin();
}

void refresh_display() {
    refresh();
}