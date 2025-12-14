#include "statistics.h"
#include "display.h"
#include "input.h"

// Load statistics from file
void loadStatistics(Statistics* stats) {
    FILE* file = fopen(STATS_FILE, "rb");
    if (file != NULL) {
        fread(stats, sizeof(Statistics), 1, file);
        fclose(file);
    } else {
        // Initialize with default values
        stats->easy.gamesWon = 0;
        stats->easy.gamesLost = 0;
        stats->easy.bestTime = -1;
        stats->easy.totalTime = 0;
        
        stats->medium.gamesWon = 0;
        stats->medium.gamesLost = 0;
        stats->medium.bestTime = -1;
        stats->medium.totalTime = 0;
        
        stats->complex.gamesWon = 0;
        stats->complex.gamesLost = 0;
        stats->complex.bestTime = -1;
        stats->complex.totalTime = 0;
        
        stats->custom.gamesWon = 0;
        stats->custom.gamesLost = 0;
        stats->custom.bestTime = -1;
        stats->custom.totalTime = 0;
        
        stats->totalTimePlayed = 0;
        stats->totalFlagsPlaced = 0;
        stats->currentWinStreak = 0;
        stats->longestWinStreak = 0;
        stats->consecutiveWins = 0;
    }
}

// Save statistics to file
void saveStatistics(Statistics* stats) {
    FILE* file = fopen(STATS_FILE, "wb");
    if (file != NULL) {
        fwrite(stats, sizeof(Statistics), 1, file);
        fclose(file);
    }
}

// Get difficulty type (0=Easy, 1=Medium, 2=Complex, 3=Custom)
int getDifficultyType(GameConfig config) {
    if (config.width == PRESET_EASY.width && config.height == PRESET_EASY.height && config.mines == PRESET_EASY.mines) {
        return 0; // Easy
    } else if (config.width == PRESET_MEDIUM.width && config.height == PRESET_MEDIUM.height && config.mines == PRESET_MEDIUM.mines) {
        return 1; // Medium
    } else if (config.width == PRESET_COMPLEX.width && config.height == PRESET_COMPLEX.height && config.mines == PRESET_COMPLEX.mines) {
        return 2; // Complex
    } else {
        return 3; // Custom
    }
}

// Update statistics after game
void updateStatistics(Statistics* stats, GameConfig config, bool won, int time, int flagsPlaced, bool godMode) {
    int difficulty = getDifficultyType(config);
    DifficultyStats* diffStats = NULL;
    
    switch (difficulty) {
        case 0: diffStats = &stats->easy; break;
        case 1: diffStats = &stats->medium; break;
        case 2: diffStats = &stats->complex; break;
        case 3: diffStats = &stats->custom; break;
    }
    
    if (diffStats != NULL) {
        diffStats->totalTime += time;
        stats->totalTimePlayed += time;
        stats->totalFlagsPlaced += flagsPlaced;
        
        if (won) {
            diffStats->gamesWon++;
            stats->currentWinStreak++;
            stats->consecutiveWins++;
            if (stats->currentWinStreak > stats->longestWinStreak) {
                stats->longestWinStreak = stats->currentWinStreak;
            }
            // Update best time
            if (diffStats->bestTime == -1 || time < diffStats->bestTime) {
                diffStats->bestTime = time;
            }
        } else {
            // Only count losses if god mode is off
            if (!godMode) {
                diffStats->gamesLost++;
                stats->currentWinStreak = 0;
                stats->consecutiveWins = 0;
            }
        }
    }
    
    saveStatistics(stats);
}

// Helper function to format time
void formatTime(int seconds, char* buffer, int bufferSize) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    snprintf(buffer, bufferSize, "%d:%02d:%02d", hours, minutes, secs);
}

// Display statistics for a difficulty
void displayDifficultyStats(const char* name, DifficultyStats* diffStats, int y, WORD color) {
    SetConsoleTextAttribute(hConsole, color | FOREGROUND_INTENSITY);
    gotoxy(0, y);
    printf("========================================\n");
    printf("          %s\n", name);
    printf("========================================\n");
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    gotoxy(0, y + 3);
    printf("  Wins:       ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("%d\n", diffStats->gamesWon);
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    gotoxy(0, y + 4);
    printf("  Losses:     ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("%d\n", diffStats->gamesLost);
    
    int totalGames = diffStats->gamesWon + diffStats->gamesLost;
    float winRate = 0.0f;
    if (totalGames > 0) {
        winRate = (float)diffStats->gamesWon / totalGames * 100.0f;
    }
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    gotoxy(0, y + 5);
    printf("  Win Rate:   ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("%.1f%%\n", winRate);
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    gotoxy(0, y + 6);
    printf("  Best Time:  ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    if (diffStats->bestTime == -1) {
        printf("No wins yet\n");
    } else {
        char timeStr[20];
        formatTime(diffStats->bestTime, timeStr, sizeof(timeStr));
        printf("%s (%d sec)\n", timeStr, diffStats->bestTime);
    }
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    gotoxy(0, y + 7);
    printf("  Total Time: ");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    char timeStr[20];
    formatTime(diffStats->totalTime, timeStr, sizeof(timeStr));
    printf("%s (%d sec)\n", timeStr, diffStats->totalTime);
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Display statistics screen
void showStatistics() {
    Statistics stats;
    loadStatistics(&stats);
    
    bool viewing = true;
    
    while (viewing) {
        clearScreen();
        
        // Header
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        gotoxy(0, 2);
        printf("========================================\n");
        printf("          STATISTICS\n");
        printf("========================================\n\n");
        
        // General statistics
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        gotoxy(0, 6);
        printf("========================================\n");
        printf("          GENERAL STATISTICS\n");
        printf("========================================\n");
        
        SetConsoleTextAttribute(hConsole, (FOREGROUND_GREEN | FOREGROUND_BLUE) | FOREGROUND_INTENSITY);
        gotoxy(0, 9);
        printf("  Total Time Played: ");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        char totalTimeStr[20];
        formatTime(stats.totalTimePlayed, totalTimeStr, sizeof(totalTimeStr));
        int hours = stats.totalTimePlayed / 3600;
        int minutes = (stats.totalTimePlayed % 3600) / 60;
        int seconds = stats.totalTimePlayed % 60;
        printf("%s (%d h %d m %d s)\n", totalTimeStr, hours, minutes, seconds);
        
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        // Easy difficulty - Green
        displayDifficultyStats("EASY", &stats.easy, 12, FOREGROUND_GREEN);
        
        // Medium difficulty - Yellow (Red + Green)
        displayDifficultyStats("MEDIUM", &stats.medium, 21, FOREGROUND_RED | FOREGROUND_GREEN);
        
        // Complex difficulty - Red
        displayDifficultyStats("COMPLEX", &stats.complex, 30, FOREGROUND_RED);
        
        // Custom difficulty - Magenta (Red + Blue)
        displayDifficultyStats("CUSTOM", &stats.custom, 39, FOREGROUND_RED | FOREGROUND_BLUE);
        
        // Footer
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        gotoxy(0, 48);
        printf("\nPress ESC to return to menu\n");
        
        int key = getKeyPress();
        if (key == VK_ESCAPE) {
            viewing = false;
        }
    }
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
