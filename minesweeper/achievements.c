#include "achievements.h"
#include "display.h"
#include "input.h"
#include "statistics.h"
#include "store.h"

// Load achievements from file
void loadAchievements(Achievements* achievements) {
    FILE* file = fopen(ACHIEVEMENTS_FILE, "rb");
    if (file != NULL) {
        fread(achievements, sizeof(Achievements), 1, file);
        fclose(file);
    } else {
        // Initialize with all achievements not completed
        for (int i = 0; i < NUM_ACHIEVEMENTS; i++) {
            achievements->completed[i] = false;
        }
    }
}

// Save achievements to file
void saveAchievements(Achievements* achievements) {
    FILE* file = fopen(ACHIEVEMENTS_FILE, "wb");
    if (file != NULL) {
        fwrite(achievements, sizeof(Achievements), 1, file);
        fclose(file);
    }
}

// Check and update achievements
void checkAchievements(Achievements* achievements, Statistics* stats, GameConfig config, bool won, int time, int flagsPlaced, bool godMode) {
    // Skip all achievement checks if god mode is enabled
    if (godMode) {
        return;
    }
    
    bool newAchievement = false;
    int coinsEarned = 0;
    
    // Achievement 0: First Win
    if (!achievements->completed[0] && won) {
        achievements->completed[0] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(0);
    }
    
    // Achievement 1: Easy Master (Win 10 Easy games)
    if (!achievements->completed[1] && won && getDifficultyType(config) == 0 && stats->easy.gamesWon >= 10) {
        achievements->completed[1] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(1);
    }
    
    // Achievement 2: Medium Master (Win 10 Medium games)
    if (!achievements->completed[2] && won && getDifficultyType(config) == 1 && stats->medium.gamesWon >= 10) {
        achievements->completed[2] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(2);
    }
    
    // Achievement 3: Complex Master (Win 10 Complex games)
    if (!achievements->completed[3] && won && getDifficultyType(config) == 2 && stats->complex.gamesWon >= 10) {
        achievements->completed[3] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(3);
    }
    
    // Achievement 4: Speed Demon (Win Easy in under 30 seconds)
    if (!achievements->completed[4] && won && getDifficultyType(config) == 0 && time < 30) {
        achievements->completed[4] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(4);
    }
    
    // Achievement 5: Lightning Fast (Win Medium in under 60 seconds)
    if (!achievements->completed[5] && won && getDifficultyType(config) == 1 && time < 60) {
        achievements->completed[5] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(5);
    }
    
    // Achievement 6: Impossible Speed (Win Complex in under 120 seconds)
    if (!achievements->completed[6] && won && getDifficultyType(config) == 2 && time < 120) {
        achievements->completed[6] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(6);
    }
    
    // Achievement 7: Perfect Game (Win without placing flags)
    if (!achievements->completed[7] && won && flagsPlaced == 0) {
        achievements->completed[7] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(7);
    }
    
    // Achievement 8: Flag Master (Place 100 flags total)
    if (!achievements->completed[8] && stats->totalFlagsPlaced >= 100) {
        achievements->completed[8] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(8);
    }
    
    // Achievement 9: Century Club (Play 100 games total)
    int totalGames = stats->easy.gamesWon + stats->easy.gamesLost +
                     stats->medium.gamesWon + stats->medium.gamesLost +
                     stats->complex.gamesWon + stats->complex.gamesLost +
                     stats->custom.gamesWon + stats->custom.gamesLost;
    if (!achievements->completed[9] && totalGames >= 100) {
        achievements->completed[9] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(9);
    }
    
    // Achievement 10: Win Streak (Win 5 games in a row)
    if (!achievements->completed[10] && stats->currentWinStreak >= 5) {
        achievements->completed[10] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(10);
    }
    
    // Achievement 11: Marathon (Play for 1 hour total)
    if (!achievements->completed[11] && stats->totalTimePlayed >= 3600) {
        achievements->completed[11] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(11);
    }
    
    // Achievement 12: No Mistakes (Win 10 games without losing)
    if (!achievements->completed[12] && stats->consecutiveWins >= 10) {
        achievements->completed[12] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(12);
    }
    
    // Achievement 13: All Difficulties (Win at least once on each difficulty)
    if (!achievements->completed[13] && won) {
        if (stats->easy.gamesWon > 0 && stats->medium.gamesWon > 0 && 
            stats->complex.gamesWon > 0 && stats->custom.gamesWon > 0) {
            achievements->completed[13] = true;
            newAchievement = true;
            coinsEarned += calculateAchievementCoins(13);
        }
    }
    
    // Achievement 14: Time Master (Win any game in under 20 seconds)
    if (!achievements->completed[14] && won && time < 20) {
        achievements->completed[14] = true;
        newAchievement = true;
        coinsEarned += calculateAchievementCoins(14);
    }
    
    if (newAchievement) {
        saveAchievements(achievements);
        if (coinsEarned > 0) {
            awardCoins(coinsEarned);
        }
    }
}

// Display achievements screen
void showAchievements() {
    Achievements achievements;
    loadAchievements(&achievements);
    
    bool viewing = true;
    
    while (viewing) {
        clearScreen();
        
        // Header
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        gotoxy(0, 2);
        printf("========================================\n");
        printf("          ACHIEVEMENTS\n");
        printf("========================================\n\n");
        
        // Count completed achievements
        int completedCount = 0;
        for (int i = 0; i < NUM_ACHIEVEMENTS; i++) {
            if (achievements.completed[i]) completedCount++;
        }
        
        // Show progress
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        gotoxy(0, 6);
        printf("Progress: %d/%d achievements completed\n\n", completedCount, NUM_ACHIEVEMENTS);
        
        // Display achievements
        int y = 8;
        for (int i = 0; i < NUM_ACHIEVEMENTS; i++) {
            gotoxy(0, y);
            
            if (achievements.completed[i]) {
                // Completed - green
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("  [X] ");
            } else {
                // Not completed - gray (using normal white/gray color)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("  [ ] ");
            }
            
            // Achievement name
            if (achievements.completed[i]) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            printf("%s\n", achievement_names[i]);
            
            // Achievement description (indented)
            gotoxy(0, y + 1);
            if (achievements.completed[i]) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            printf("      %s\n", achievement_descriptions[i]);
            y += 2; // Move to next achievement (name + description = 2 lines)
        }
        
        // Reset color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        gotoxy(0, 50);
        printf("\nPress ESC to return to menu\n");
        
        int key = getKeyPress();
        if (key == VK_ESCAPE) {
            viewing = false;
        }
    }
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
