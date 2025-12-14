#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"
#include "game.h"

// Display/UI functions
void gotoxy(int x, int y);
void clearScreen();
void displayGame(GameState* game, bool godMode, bool hasDetector, bool hasShield, bool hasScanner);
void showGameOver(GameState* game, bool won, bool godMode, bool hasDetector, bool hasShield, bool hasScanner, int coinsEarned);

#endif // DISPLAY_H
