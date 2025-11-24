#include "input_handler.h"
#include "../display/display.h"
#include <stdlib.h>

void handle_input(InputCode input) {
    switch (game.state) {
        case GAME_MAIN_MENU:
            handle_menu_input(input);
            break;
        case GAME_DIFFICULTY_MENU:
            handle_difficulty_input(input);
            break;
        case GAME_CUSTOM_MENU:
            handle_custom_input(input);
            break;
        case GAME_PLAYING:
            handle_game_input(input);
            break;
        case GAME_WON:
        case GAME_LOST:
            handle_game_over_input(input);
            break;
    }
}

void handle_menu_input(InputCode input) {
    switch (input) {
        case INPUT_UP:
            if (game.selected_menu_item > 0) {
                game.selected_menu_item--;
            }
            break;
            
        case INPUT_DOWN:
            if (game.selected_menu_item < 2) {
                game.selected_menu_item++;
            }
            break;
            
        case INPUT_SELECT:
            switch (game.selected_menu_item) {
                case 0: // Start
                    game.state = GAME_DIFFICULTY_MENU;
                    game.selected_menu_item = 0;
                    break;
                case 1: // Statistics
                    // TODO: Реализовать статистику
                    break;
                case 2: // Quit
                    exit(0);
                    break;
            }
            break;
            
        case INPUT_QUIT:
            exit(0);
            break;
            
        default:
            break;
    }
}

void handle_game_input(InputCode input) {
    switch (input) {
        case INPUT_UP:
            if (game.cursor_y > 0) {
                game.cursor_y--;
            }
            break;
            
        case INPUT_DOWN:
            if (game.cursor_y < game.height - 1) {
                game.cursor_y++;
            }
            break;
            
        case INPUT_LEFT:
            if (game.cursor_x > 0) {
                game.cursor_x--;
            }
            break;
            
        case INPUT_RIGHT:
            if (game.cursor_x < game.width - 1) {
                game.cursor_x++;
            }
            break;
            
        case INPUT_SELECT:
            if (!game.field[game.cursor_y][game.cursor_x].is_flagged) {
                reveal_cell(game.cursor_x, game.cursor_y);
                
                if (is_game_over()) {
                    game.state = GAME_LOST;
                } else if (check_win()) {
                    game.state = GAME_WON;
                }
            }
            break;
            
        case INPUT_FLAG:
            if (!game.field[game.cursor_y][game.cursor_x].is_revealed) {
                toggle_flag(game.cursor_x, game.cursor_y);
            }
            break;
            
        case INPUT_BACK:
            cleanup_game();
            game.state = GAME_MAIN_MENU;
            break;
            
        case INPUT_QUIT:
            cleanup_game();
            exit(0);
            break;
            
        default:
            break;
    }
}

void handle_game_over_input(InputCode input) {
    if (input != INPUT_NONE) {
        cleanup_game();
        game.state = GAME_MAIN_MENU;
    }
}

void handle_difficulty_input(InputCode input) {
    switch (input) {
        case INPUT_UP:
            if (game.selected_menu_item > 0) {
                game.selected_menu_item--;
            }
            break;
            
        case INPUT_DOWN:
            if (game.selected_menu_item < 3) { // Easy, Normal, Hard, Custom
                game.selected_menu_item++;
            }
            break;
            
        case INPUT_SELECT:
            switch (game.selected_menu_item) {
                case 0: // Easy
                    initialize_game(8, 8, 10);
                    game.state = GAME_PLAYING;
                    break;
                case 1: // Normal
                    initialize_game(12, 12, 20);
                    game.state = GAME_PLAYING;
                    break;
                case 2: // Hard
                    initialize_game(16, 16, 40);
                    game.state = GAME_PLAYING;
                    break;
                case 3: // Custom
                    game.state = GAME_CUSTOM_MENU;
                    game.custom_width = 2;
                    game.custom_height = 2;
                    game.custom_mines = 1;
                    game.custom_cursor = 0;
                    break;
            }
            break;
            
        case INPUT_BACK:
            game.state = GAME_MAIN_MENU;
            game.selected_menu_item = 0;
            break;
            
        case INPUT_QUIT:
            exit(0);
            break;
            
        default:
            break;
    }
}

void handle_custom_input(InputCode input) {
    switch (input) {
        case INPUT_UP:
            if (game.custom_cursor > 0) {
                game.custom_cursor--;
            }
            break;
            
        case INPUT_DOWN:
            if (game.custom_cursor < 2) { // Width, Height, Mines
                game.custom_cursor++;
            }
            break;
            
        case INPUT_LEFT:
            switch (game.custom_cursor) {
                case 0: // Width
                    if (game.custom_width > 2) game.custom_width--;
                    break;
                case 1: // Height
                    if (game.custom_height > 2) game.custom_height--;
                    break;
                case 2: // Mines
                    if (game.custom_mines > 1) game.custom_mines--;
                    break;
            }
            break;
            
        case INPUT_RIGHT:
            switch (game.custom_cursor) {
                case 0: // Width
                    if (game.custom_width < 60) game.custom_width++;
                    break;
                case 1: // Height
                    if (game.custom_height < 60) game.custom_height++;
                    break;
                case 2: // Mines
                    if (game.custom_mines < game.custom_width * game.custom_height - 1) {
                        game.custom_mines++;
                    }
                    break;
            }
            break;
            
        case INPUT_SELECT:
            if (game.custom_mines < game.custom_width * game.custom_height) {
                initialize_game(game.custom_width, game.custom_height, game.custom_mines);
                game.state = GAME_PLAYING;
            }
            break;
            
        case INPUT_BACK:
            game.state = GAME_DIFFICULTY_MENU;
            break;
            
        case INPUT_QUIT:
            exit(0);
            break;
            
        default:
            break;
    }
}