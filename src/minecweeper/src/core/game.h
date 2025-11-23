#ifndef GAME_H
#define GAME_H
#define SIZE 8
#define MINES 10

extern char field[SIZE][SIZE];
extern char visible[SIZE][SIZE];
extern int mines_left;

void initialize_game();
int count_adjacent_mines(int x, int y);
void reveal(int x, int y);
void place_flag(int x, int y);
int check_win();
int is_game_over();

#endif