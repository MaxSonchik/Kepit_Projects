#ifndef MENU_H
#define MENU_H

#include "common.h"

// Menu options
extern const char* menu_items[MENU_ITEMS];
extern const char* difficulty_items[DIFFICULTY_ITEMS];

// Menu functions
void displayMenu(int selected);
void displayDifficultyMenu(int selected, GameConfig* config);
void displayCustomDifficulty(GameConfig* config, int selectedField);
GameConfig showDifficultyMenu();
GameConfig configureCustomDifficulty();

#endif // MENU_H
