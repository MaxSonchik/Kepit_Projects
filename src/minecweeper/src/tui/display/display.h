#ifndef DISPLAY_H
#define DISPLAY_H

#include "../../core/game.h"


void draw_main_menu();
void draw_difficulty_menu();
void draw_custom_menu();
void draw_game_field();
void draw_victory_screen();
void draw_game_over_screen();
void draw_current_screen();

void draw_cell(int x, int y, Cell cell, int is_cursor);
void draw_ui_panel();

#endif