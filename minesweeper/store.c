#include "store.h"
#include "display.h"
#include "input.h"
#include "statistics.h"

// Load store from file
void loadStore(Store* store) {
    FILE* file = fopen(STORE_FILE, "rb");
    if (file != NULL) {
        fread(store, sizeof(Store), 1, file);
        // Handle old save files that don't have levels
        if (store->detectorLevel == 0 && store->hasDetector) store->detectorLevel = 1;
        if (store->shieldLevel == 0 && store->hasShield) store->shieldLevel = 1;
        if (store->scannerLevel == 0 && store->hasScanner) store->scannerLevel = 1;
        fclose(file);
    } else {
        // Initialize with default values
        store->coins = 0;  // Start with 0 coins - must earn them!
        store->hasDetector = false;
        store->hasShield = false;
        store->hasScanner = false;
        store->detectorLevel = 0;
        store->shieldLevel = 0;
        store->scannerLevel = 0;
    }
}

// Save store to file
void saveStore(Store* store) {
    FILE* file = fopen(STORE_FILE, "wb");
    if (file != NULL) {
        fwrite(store, sizeof(Store), 1, file);
        fclose(file);
    }
}

// Display store screen
void showStore() {
    Store store;
    loadStore(&store);
    
    int selected = 0; // 0 = Detector, 1 = Shield, 2 = Scanner
    bool viewing = true;
    
    // Ability prices
    const int prices[NUM_ABILITIES] = {5000, 10000, 15000};
    const char* ability_names[NUM_ABILITIES] = {"Detector", "Shield", "Scanner"};
    const char* ability_descriptions[NUM_ABILITIES] = {
        "Detects nearby mines",
        "Protects from mine explosions",
        "Scans the board for mines"
    };
    
    while (viewing) {
        clearScreen();
        
        // Header
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        gotoxy(0, 2);
        printf("========================================\n");
        printf("              STORE\n");
        printf("========================================\n\n");
        
        // Display coin balance
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        gotoxy(0, 6);
        printf("Coins: ");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("%d\n\n", store.coins);
        
        // Display abilities
        for (int i = 0; i < NUM_ABILITIES; i++) {
            gotoxy(0, 8 + i * 3);
            
            // Selection indicator
            if (i == selected) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("  > ");
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("    ");
            }
            
            // Check if ability is owned
            bool owned = false;
            if (i == 0) owned = store.hasDetector;
            else if (i == 1) owned = store.hasShield;
            else if (i == 2) owned = store.hasScanner;
            
            // Ability name and status
            if (owned) {
                // Owned - show in green
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("%s [OWNED]", ability_names[i]);
            } else {
                // Not owned - show in normal color
                if (i == selected) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                } else {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
                printf("%s - %d coins", ability_names[i], prices[i]);
            }
            
            if (i == selected) {
                printf(" <");
            }
            printf("\n");
            
            // Description
            gotoxy(0, 9 + i * 3);
            if (owned) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            printf("      %s\n", ability_descriptions[i]);
        }
        
        // Reset text color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        // Instructions
        gotoxy(0, 18);
        printf("\nUse UP/DOWN to navigate\n");
        printf("Press ENTER/SPACE to purchase, ESC to return\n");
        
        int key = getKeyPress();
        
        switch (key) {
            case 'W':
            case 'w':
            case VK_UP:
                selected = (selected - 1 + NUM_ABILITIES) % NUM_ABILITIES;
                break;
                
            case 'S':
            case 's':
            case VK_DOWN:
                selected = (selected + 1) % NUM_ABILITIES;
                break;
                
            case VK_RETURN:
            case VK_SPACE:
                // Purchase ability
                bool* abilityPtr = NULL;
                int* levelPtr = NULL;
                if (selected == 0) {
                    abilityPtr = &store.hasDetector;
                    levelPtr = &store.detectorLevel;
                } else if (selected == 1) {
                    abilityPtr = &store.hasShield;
                    levelPtr = &store.shieldLevel;
                } else if (selected == 2) {
                    abilityPtr = &store.hasScanner;
                    levelPtr = &store.scannerLevel;
                }
                
                if (abilityPtr != NULL && !(*abilityPtr)) {
                    // Check if already owned
                    if (!(*abilityPtr) && store.coins >= prices[selected]) {
                        *abilityPtr = true;
                        if (levelPtr != NULL) {
                            *levelPtr = 1; // Start at level 1
                        }
                        store.coins -= prices[selected];
                        saveStore(&store);
                    }
                }
                break;
                
            case VK_ESCAPE:
                viewing = false;
                break;
        }
    }
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Calculate coins earned from a game
int calculateGameCoins(GameConfig config, bool won, int time, int flagsPlaced, Statistics* stats, bool godMode) {
    if (godMode || !won) {
        return 0; // No coins in god mode or if lost
    }
    
    int coins = 0;
    int difficulty = getDifficultyType(config);
    
    // Base coins for winning (varies by difficulty)
    switch (difficulty) {
        case 0: coins = 50; break;   // Easy: 50 coins
        case 1: coins = 150; break;  // Medium: 150 coins
        case 2: coins = 300; break;  // Complex: 300 coins
        case 3: coins = 100; break;  // Custom: 100 coins base
    }
    
    // Speed bonus - faster completion = more coins
    int speedBonus = 0;
    if (difficulty == 0 && time < 30) {  // Easy: under 30s
        speedBonus = 50 + (30 - time) * 2; // 50-110 bonus
    } else if (difficulty == 1 && time < 60) {  // Medium: under 60s
        speedBonus = 100 + (60 - time) * 3; // 100-280 bonus
    } else if (difficulty == 2 && time < 120) {  // Complex: under 120s
        speedBonus = 200 + (120 - time) * 5; // 200-800 bonus
    } else if (difficulty == 3) {
        // Custom: bonus based on board size and time
        int totalCells = config.width * config.height;
        int expectedTime = totalCells / 2; // Rough estimate
        if (time < expectedTime) {
            speedBonus = (expectedTime - time) * 2;
        }
    }
    coins += speedBonus;
    
    // Perfect game bonus (no flags used)
    if (flagsPlaced == 0) {
        coins += 100; // Perfect game bonus
    }
    
    // Win streak bonus
    if (stats->currentWinStreak > 1) {
        coins += stats->currentWinStreak * 10; // 10 coins per streak level
    }
    
    // First win bonus (if it's the first win on this difficulty)
    // Note: stats haven't been updated yet, so gamesWon is still the old value
    DifficultyStats* diffStats = NULL;
    switch (difficulty) {
        case 0: diffStats = &stats->easy; break;
        case 1: diffStats = &stats->medium; break;
        case 2: diffStats = &stats->complex; break;
        case 3: diffStats = &stats->custom; break;
    }
    if (diffStats != NULL && diffStats->gamesWon == 0) {
        coins += 50; // First win bonus
    }
    
    return coins;
}

// Calculate coins for a specific achievement
int calculateAchievementCoins(int achievementIndex) {
    // Different achievements give different coin rewards
    switch (achievementIndex) {
        case 0: return 100;   // First Win
        case 1: return 200;   // Easy Master
        case 2: return 300;   // Medium Master
        case 3: return 500;   // Complex Master
        case 4: return 150;   // Speed Demon
        case 5: return 250;   // Lightning Fast
        case 6: return 400;   // Impossible Speed
        case 7: return 200;   // Perfect Game
        case 8: return 150;   // Flag Master
        case 9: return 300;   // Century Club
        case 10: return 250;  // Win Streak
        case 11: return 200;  // Marathon
        case 12: return 350;  // No Mistakes
        case 13: return 400;  // All Difficulties
        case 14: return 300;  // Time Master
        default: return 100;  // Default reward
    }
}

// Award coins to player
void awardCoins(int amount) {
    if (amount <= 0) return;
    
    Store store;
    loadStore(&store);
    store.coins += amount;
    saveStore(&store);
}
