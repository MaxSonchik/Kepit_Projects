#ifndef GAME_H
#define GAME_H
#include <stdio.h>

typedef struct {
    bool is_mine;
    bool is_revealed;
    bool is_flagged;
    int adjacent_mines;
} Cell;

typedef struct {
    int wedth;
    int height;
    int total_mines;
    Cell **field;
} Game;

#endif