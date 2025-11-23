#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

char field[SIZE][SIZE];
char visible[SIZE][SIZE];
int mines_left;

void initialize_game() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            field[i][j] = ' ';
            visible[i][j] = '.';
        }
    }

    srand(time(NULL));
    int mines_placed = 0;
    while (mines_placed < MINES) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        if (field[x][y] != 'X') {
            field[x][y] = 'X';
            mines_placed++;
        }
    }
    mines_left = MINES;
}

int count_adjacent_mines(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE) {
                if (field[nx][ny] == 'X') count++;
            }
        }
    }
    return count;
}

void reveal(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return;
    if (visible[x][y] != '.') return;

    if (field[x][y] == 'X') {
        visible[x][y] = 'X';
        return;
    }

    int mines = count_adjacent_mines(x, y);
    if (mines > 0) {
        visible[x][y] = '0' + mines;
    } else {
        visible[x][y] = ' ';
        for (int i = -1; i <=1; i++) {
            for (int j = -1; j <= 1; j++) {
                reveal(x + i, y + j);
            }
        }
    }
}

void place_flag(int x, int y) {
    if (visible[x][y] == '.') {
        visible[x][y] = 'F';
        mines_left--;
    } else if (visible[x][y] == 'F') {
        visible[x][y] = '.';
        mines_left++;
    }
}

int check_win() {
    int correctly_flagged = 0;
    int safe_cells_revealed = 0;
    int total_safe_cells = SIZE * SIZE - MINES;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (field[i][j] != 'X' && visible[i][j] != '.' && visible[i][j] != 'F') {
                safe_cells_revealed++;
            }
            if (field[i][j] == 'X' && visible[i][j] == 'F') {
                correctly_flagged++;
            }
        }
    }
    return (safe_cells_revealed == total_safe_cells) || (correctly_flagged == MINES);
}

int is_game_over() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (visible[i][j] == 'X') return 1;
        }
    }
    return 0;
}