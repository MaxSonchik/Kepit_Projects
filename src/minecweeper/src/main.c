#include <stdio.h>
#include <stdlib.h>
#include "core/game.h"
#include "tui/core/tui.h"
#include "tui/input/input.h"
#include "tui/input/input_handler.h"
#include "tui/display/display.h"


int main() {
    init_tui();
    
    game.state = GAME_MAIN_MENU;
    game.selected_menu_item = 0;
    game.width = 0;
    game.height = 0;
    game.total_mines = 0;
    game.flags_placed = 0;
    game.cells_revealed = 0;
    game.cursor_x = 0;
    game.cursor_y = 0;
    game.field = NULL;
    game.start_time = 0;
    game.elapsed_time = 0;
    game.custom_width = 0;
    game.custom_height = 0;
    game.custom_mines = 0;
    game.custom_cursor = 0;

    int running = 1;
    while (running) {
        draw_current_screen();
        
        InputCode input = get_input();
        if (input == INPUT_QUIT) {
            running = 0;
        } else {
            handle_input(input);
        }
    }
    
    cleanup_game();
    cleanup_tui();
    
    printf("Спасибо за игру!\n");
    return 0;
}