#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <time.h>

#define BEGINNER_SIZE 8
#define BEGINNER_MINES 10
#define INTERMEDIATE_SIZE 12
#define INTERMEDIATE_MINES 20
#define EXPERT_SIZE 16  
#define EXPERT_MINES 40

typedef enum {
    GAME_MAIN_MENU,
    GAME_DIFFICULTY_MENU,
    GAME_CUSTOM_MENU,
    GAME_PLAYING,
    GAME_WON,
    GAME_LOST
} GameState;

typedef struct {
    int width;
    int height;
    int mines;
} CustomSettings;
typedef struct {
    bool is_mine;
    bool is_revealed;
    bool is_flagged;
    int adjacent_mines;
} Cell;

typedef struct {
    int width;
    int height;
    int total_mines;
    Cell **field;
    int flags_placed;
    int cells_revealed;
    GameState state;
    int cursor_x;
    int cursor_y;

    int selected_menu_item;
    int custom_width;
    int custom_height;
    int custom_mines;
    int custom_cursor;

    time_t start_time;
    time_t elapsed_time;
} Game;

extern Game game;

void initialize_game(int width, int height, int mines);
void cleanup_game();
void reveal_cell(int x, int y);
void toggle_flag(int x, int y);
bool check_win();
bool is_game_over();
int count_adjacent_mines(int x, int y);
void start_game_timer();
time_t get_game_time();

#endif