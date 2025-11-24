#include "display.h"
#include "../core/tui.h"
#include <ncurses.h>

void draw_main_menu() {
    clear();
    
    attron(A_BOLD);
    mvprintw(2, 5, "=== MINESWEEPER ===");
    attroff(A_BOLD);
    
    const char *menu_items[] = {
        "Start Game",
        "Statistics",
        "Quit"
    };
    
    for (int i = 0; i < 3; i++) {
        if (i == game.selected_menu_item) {
            attron(A_REVERSE);
        }
        
        mvprintw(5 + i, 5, "%s", menu_items[i]);
        
        if (i == game.selected_menu_item) {
            attroff(A_REVERSE);
        }
    }
    
    mvprintw(12, 5, "Use ARROWS to navigate, ENTER to select, Q to quit");
    
    refresh();
}

void draw_current_screen() {
    switch (game.state) {
        case GAME_MAIN_MENU:
            draw_main_menu();
            break;
        case GAME_DIFFICULTY_MENU:
            draw_difficulty_menu();
            break;
        case GAME_CUSTOM_MENU:
            draw_custom_menu();
            break;
        case GAME_PLAYING:
            draw_game_field();
            break;
        case GAME_WON:
            draw_victory_screen();
            break;
        case GAME_LOST:
            draw_game_over_screen();
            break;
    }
}

void draw_cell(int x, int y, Cell cell, int is_cursor) {
    int screen_x = x * 2 + 5;
    int screen_y = y + 3;

    if (is_cursor) {
        attron(A_REVERSE);
    }
    
    char display_char = ' ';
    if (!cell.is_revealed) {
        if (cell.is_flagged) {
            display_char = 'F';
        } else {
            display_char = '.';
        }
    } else {
        if (cell.is_mine) {
            display_char = 'X';
        } else if (cell.adjacent_mines > 0) {
            display_char = '0' + cell.adjacent_mines;
        } else {
            display_char = ' ';
        }
    }
    
    mvprintw(screen_y, screen_x, "%c ", display_char);
    
    if (is_cursor) {
        attroff(A_REVERSE);
    }
}

void draw_game_field() {
    clear();

    attron(A_BOLD);
    mvprintw(1, 5, "=== MINESWEEPER ===");
    attroff(A_BOLD);

    if (game.field != NULL) {
        for (int y = 0; y < game.height; y++) {
            for (int x = 0; x < game.width; x++) {
                int is_cursor = (x == game.cursor_x && y == game.cursor_y);
                draw_cell(x, y, game.field[y][x], is_cursor);
            }
        }
    } else {
        mvprintw(5, 5, "No game field!");
    }
    int info_y = game.height + 5;
    mvprintw(info_y, 5, "Mines left: %d", game.total_mines - game.flags_placed);
    mvprintw(info_y + 1, 5, "Time: 00:00");
    mvprintw(info_y + 3, 5, "Controls: Arrows-move, Space-open, F-flag, Q-quit");
    
    refresh();
}

void draw_victory_screen() {
    clear();
    mvprintw(5, 5, "*** VICTORY! ***");
    mvprintw(7, 5, "All mines cleared!");
    mvprintw(9, 5, "Press any key to return to menu");
    refresh();
}

void draw_game_over_screen() {
    clear();
    mvprintw(5, 5, "*** GAME OVER ***");
    mvprintw(7, 5, "You hit a mine!");
    mvprintw(9, 5, "Press any key to return to menu");
    refresh();
}

void draw_ui_panel() {
    int info_y = game.height + 5;
    mvprintw(info_y, 5, "Mines: %d/%d", game.flags_placed, game.total_mines);
}

void draw_difficulty_menu() {
    clear();
    
    attron(A_BOLD);
    mvprintw(2, 5, "=== SELECT DIFFICULTY ===");
    attroff(A_BOLD);
    
    const char *difficulty_items[] = {
        "Easy (8x8, 10 mines)",
        "Normal (12x12, 20 mines)", 
        "Hard (16x16, 40 mines)",
        "Custom"
    };
    
    for (int i = 0; i < 4; i++) {
        if (i == game.selected_menu_item) {
            attron(A_REVERSE);
        }
        
        mvprintw(5 + i, 5, "%s", difficulty_items[i]);
        
        if (i == game.selected_menu_item) {
            attroff(A_REVERSE);
        }
    }
    
    mvprintw(12, 5, "Use ARROWS to navigate, ENTER to select, ESC to go back");
    
    refresh();
}

void draw_custom_menu() {
    clear();
    
    attron(A_BOLD);
    mvprintw(2, 5, "=== CUSTOM SETTINGS ===");
    attroff(A_BOLD);

    const char *field_names[] = {"Width", "Height", "Mines"};
    int values[] = {game.custom_width, game.custom_height, game.custom_mines};
    
    for (int i = 0; i < 3; i++) {
        if (i == game.custom_cursor) {
            attron(A_REVERSE);
        }
        
        mvprintw(5 + i, 5, "%s: %d", field_names[i], values[i]);
        
        if (i == game.custom_cursor) {
            attroff(A_REVERSE);
        }
    }
    
    mvprintw(9, 5, "Preview: %dx%d field", game.custom_width, game.custom_height);
    mvprintw(10, 5, "Mines: %d (%.1f%%)", game.custom_mines, 
             (float)game.custom_mines / (game.custom_width * game.custom_height) * 100);
    
    mvprintw(12, 5, "Max size: 60x60, Min mines: 1, Max mines: %d", 
             game.custom_width * game.custom_height - 1);
    
             
    mvprintw(14, 5, "Controls: UP/DOWN - select field, LEFT/RIGHT - change value");
    mvprintw(15, 5, "ENTER - start game, ESC - go back");
    
    refresh();
}