#include "common.h"

// Preset difficulties
const GameConfig PRESET_EASY = {9, 9, 10};
const GameConfig PRESET_MEDIUM = {16, 16, 40};
const GameConfig PRESET_COMPLEX = {30, 16, 99};

// Achievement names and descriptions
const char* achievement_names[NUM_ACHIEVEMENTS] = {
    "First Win",                    // 0: Win your first game
    "Easy Master",                  // 1: Win 10 Easy games
    "Medium Master",                // 2: Win 10 Medium games
    "Complex Master",               // 3: Win 10 Complex games
    "Speed Demon",                  // 4: Win Easy in under 30 seconds
    "Lightning Fast",               // 5: Win Medium in under 60 seconds
    "Impossible Speed",             // 6: Win Complex in under 120 seconds
    "Perfect Game",                // 7: Win without placing any flags
    "Flag Master",                 // 8: Place 100 flags total
    "Century Club",                // 9: Play 100 games total
    "Win Streak",                  // 10: Win 5 games in a row
    "Marathon",                    // 11: Play for 1 hour total
    "No Mistakes",                 // 12: Win 10 games without losing
    "All Difficulties",            // 13: Win at least once on each difficulty
    "Time Master"                  // 14: Win a game in under 20 seconds
};

const char* achievement_descriptions[NUM_ACHIEVEMENTS] = {
    "Win your first game",
    "Win 10 games on Easy difficulty",
    "Win 10 games on Medium difficulty",
    "Win 10 games on Complex difficulty",
    "Win Easy difficulty in under 30 seconds",
    "Win Medium difficulty in under 60 seconds",
    "Win Complex difficulty in under 120 seconds",
    "Win a game without placing any flags",
    "Place 100 flags across all games",
    "Play 100 games total",
    "Win 5 games in a row",
    "Play for 1 hour total",
    "Win 10 games without losing",
    "Win at least once on each difficulty (Easy, Medium, Complex, Custom)",
    "Win any game in under 20 seconds"
};

// Console handles
HANDLE hConsole;
COORD cursorPos;
