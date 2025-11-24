#include "game.h"
#include <stdlib.h>
#include <time.h>

Game game;

void initialize_game(int width, int height, int mines) {
    cleanup_game();

    game.width = width;
    game.height = height;
    game.total_mines = mines;
    game.flags_placed = 0;
    game.cells_revealed = 0;
    game.state = GAME_PLAYING;
    game.cursor_x = 0;
    game.cursor_y = 0;

    game.field = (Cell**)malloc(height * sizeof(Cell*));
    for (int i = 0; i < height; i++) {
        game.field[i] = (Cell*)malloc(width * sizeof(Cell));
        for (int j = 0; j < width; j++) {
            game.field[i][j].is_mine = false;
            game.field[i][j].is_revealed = false;
            game.field[i][j].is_flagged = false;
            game.field[i][j].adjacent_mines = 0;
        }
    }

    srand(time(NULL));
    int mines_placed = 0;
    
    while (mines_placed < mines) {
        int x = rand() % width;
        int y = rand() % height;

        if (!game.field[y][x].is_mine) {
            game.field[y][x].is_mine = true;
            mines_placed++;
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!game.field[y][x].is_mine) {
                game.field[y][x].adjacent_mines = count_adjacent_mines(x, y);
            }
        }
    }
}

void cleanup_game() {
    if (game.field != NULL) {
        for (int i = 0; i < game.height; i++) {
            free(game.field[i]);
        }
        free(game.field);
        game.field = NULL;
    }
}

void reveal_cell(int x, int y) {
    if (x < 0 || x >= game.width || y < 0 || y >= game.height) {
        return;
    }
    
    Cell *cell = &game.field[y][x];

    if (cell->is_revealed || cell->is_flagged) {
        return;
    }

    cell->is_revealed = true;
    game.cells_revealed++;

    if (cell->is_mine) {
        game.state = GAME_LOST;
        return;
    }

    if (cell->adjacent_mines == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                
                reveal_cell(x + dx, y + dy);
            }
        }
    }
}

void toggle_flag(int x, int y) {
    if (x >= 0 && x < game.width && y >= 0 && y < game.height) {
        if (!game.field[y][x].is_revealed) {
            game.field[y][x].is_flagged = !game.field[y][x].is_flagged;
            if (game.field[y][x].is_flagged) {
                game.flags_placed++;
            } else {
                game.flags_placed--;
            }
        }
    }
}

bool check_win() {
    for (int y = 0; y < game.height; y++) {
        for (int x = 0; x < game.width; x++) {
            Cell cell = game.field[y][x];
            if (!cell.is_mine && !cell.is_revealed) {
                return false;
            }
        }
    }
    return true;
}

bool is_game_over() {
    for (int y = 0; y < game.height; y++) {
        for (int x = 0; x < game.width; x++) {
            Cell cell = game.field[y][x];
            if (cell.is_mine && cell.is_revealed) {
                return true;
            }
        }
    }
    return false;
}


int count_adjacent_mines(int x, int y) {
    int count = 0;

    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int nx = x + dx;
            int ny = y + dy;

            if ((dx == 0 && dy == 0) || 
                nx < 0 || nx >= game.width || 
                ny < 0 || ny >= game.height) {
                continue;
            }

            if (game.field[ny][nx].is_mine) {
                count++;
            }
        }
    }
    
    return count;
}

void start_game_timer() {
    // TODO: Реализовать таймер
}

time_t get_game_time() {
    // TODO: Реализовать получение времени
    return 0;
}