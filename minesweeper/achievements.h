#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "common.h"

// Achievements functions
void loadAchievements(Achievements* achievements);
void saveAchievements(Achievements* achievements);
void checkAchievements(Achievements* achievements, Statistics* stats, GameConfig config, bool won, int time, int flagsPlaced, bool godMode);
void showAchievements();

#endif // ACHIEVEMENTS_H
