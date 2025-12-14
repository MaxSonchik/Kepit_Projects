#ifndef STATISTICS_H
#define STATISTICS_H

#include "common.h"

// Statistics functions
void loadStatistics(Statistics* stats);
void saveStatistics(Statistics* stats);
void updateStatistics(Statistics* stats, GameConfig config, bool won, int time, int flagsPlaced, bool godMode);
void showStatistics();
int getDifficultyType(GameConfig config);
void formatTime(int seconds, char* buffer, int bufferSize);
void displayDifficultyStats(const char* name, DifficultyStats* diffStats, int y, WORD color);

#endif // STATISTICS_H
