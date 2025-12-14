#ifndef COMMON_H
#define COMMON_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Constants
#define MENU_ITEMS 6
#define DIFFICULTY_ITEMS 4
#define MAX_TIME 999  // Maximum time in seconds (999 = no limit, or set a specific limit)
#define STATS_FILE "minesweeper_stats.dat"
#define ACHIEVEMENTS_FILE "minesweeper_achievements.dat"
#define SETTINGS_FILE "minesweeper_settings.dat"
#define STORE_FILE "minesweeper_store.dat"
#define NUM_ACHIEVEMENTS 15
#define NUM_ABILITIES 3
#define NUM_THEMES 4

// Game configuration structure
typedef struct {
    int width;
    int height;
    int mines;
} GameConfig;

// Cell structure
typedef struct {
    bool hasMine;
    bool isRevealed;
    bool isFlagged;
    int adjacentMines;
    bool isDetected;  // Shown by detector ability
    bool isScanned;   // Shown by scanner ability
} Cell;

// Modifier types
typedef enum {
    MODIFIER_NONE = 0,
    MODIFIER_RANDOM_ABILITY = 1,
    MODIFIER_DISABLE_ABILITIES = 2,
    MODIFIER_DISABLE_TIMER = 3
} ModifierType;

// Game state structure
typedef struct {
    Cell** board;
    GameConfig config;
    int cursorX;
    int cursorY;
    bool gameStarted;
    bool gameOver;
    bool gameWon;
    time_t startTime;
    int elapsedSeconds;
    int flagsPlaced;
    int cellsRevealed;
    // Ability states
    bool shieldActive;
    time_t shieldStartTime;
    int shieldDuration;  // Duration in seconds
    // Ability cooldowns (1 minute = 60 seconds)
    time_t detectorLastUsed;
    time_t shieldLastUsed;
    time_t scannerLastUsed;
    // Modifier state
    ModifierType activeModifier;
    time_t lastRandomAbilityTime;  // For random ability modifier
} GameState;

// Statistics for each difficulty level
typedef struct {
    int gamesWon;          // Number of wins
    int gamesLost;         // Number of losses
    int bestTime;          // Best time in seconds (-1 if no win)
    int totalTime;         // Total time played in seconds
} DifficultyStats;

// Statistics structure
typedef struct {
    DifficultyStats easy;      // Easy difficulty stats
    DifficultyStats medium;    // Medium difficulty stats
    DifficultyStats complex;  // Complex difficulty stats
    DifficultyStats custom;   // Custom difficulty stats
    int totalTimePlayed;       // Total time played in seconds (all difficulties)
    int totalFlagsPlaced;      // Total flags placed across all games
    int currentWinStreak;      // Current win streak
    int longestWinStreak;       // Longest win streak
    int consecutiveWins;       // Consecutive wins without losses
} Statistics;

// Achievements structure
typedef struct {
    bool completed[NUM_ACHIEVEMENTS];
} Achievements;

// Settings structure
typedef struct {
    bool godMode;
    bool modifiersEnabled;
    int themeIndex;  // Index into themes array (0 = Default)
} Settings;

// Store structure
typedef struct {
    int coins;
    bool hasDetector;
    bool hasShield;
    bool hasScanner;
    int detectorLevel;   // 1-5 (shows 1-5 mines)
    int shieldLevel;     // 1-8 (3-10 seconds protection)
    int scannerLevel;    // 1-3 (2x2, 3x3, 4x4 area)
} Store;

// Preset difficulties
extern const GameConfig PRESET_EASY;
extern const GameConfig PRESET_MEDIUM;
extern const GameConfig PRESET_COMPLEX;

// Achievement names and descriptions
extern const char* achievement_names[NUM_ACHIEVEMENTS];
extern const char* achievement_descriptions[NUM_ACHIEVEMENTS];

// Console handles (extern declarations)
extern HANDLE hConsole;
extern COORD cursorPos;

#endif // COMMON_H
