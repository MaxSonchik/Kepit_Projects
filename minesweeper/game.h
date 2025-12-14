#ifndef GAME_H
#define GAME_H

#include "common.h"

// Game logic functions
void startGame(GameConfig config, bool godMode);
void initializeGame(GameState* game);
void generateMines(GameState* game, int firstX, int firstY);
int countAdjacentMines(GameState* game, int x, int y);
void revealCell(GameState* game, int x, int y, bool godMode);
void revealAdjacentCells(GameState* game, int x, int y, bool godMode);
void toggleFlag(GameState* game, int x, int y);
bool checkWin(GameState* game);
void cleanupGame(GameState* game);

#endif // GAME_H
