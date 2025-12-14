#ifndef STORE_H
#define STORE_H

#include "common.h"

// Store functions
void loadStore(Store* store);
void saveStore(Store* store);
void showStore();
int calculateGameCoins(GameConfig config, bool won, int time, int flagsPlaced, Statistics* stats, bool godMode);
int calculateAchievementCoins(int achievementIndex);
void awardCoins(int amount);

#endif // STORE_H
