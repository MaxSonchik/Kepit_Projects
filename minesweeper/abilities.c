#include "abilities.h"

// Get ability cooldown remaining in seconds (0 = ready, >0 = cooldown remaining)
// abilityType: 0=Detector, 1=Shield, 2=Scanner
int getAbilityCooldown(GameState* game, int abilityType) {
    time_t currentTime = time(NULL);
    time_t lastUsed = 0;
    
    switch (abilityType) {
        case 0: lastUsed = game->detectorLastUsed; break;
        case 1: lastUsed = game->shieldLastUsed; break;
        case 2: lastUsed = game->scannerLastUsed; break;
        default: return 0;
    }
    
    if (lastUsed == 0) return 0; // Never used, ready
    
    int elapsed = (int)(currentTime - lastUsed);
    int cooldownTime = 60; // 1 minute cooldown
    
    if (elapsed >= cooldownTime) {
        return 0; // Cooldown expired, ready
    }
    
    return cooldownTime - elapsed; // Return remaining cooldown
}

// Activate Detector ability - shows 1-5 random mines based on level
// Returns true if activated, false if on cooldown
bool activateDetector(GameState* game, int level) {
    // Check cooldown
    if (getAbilityCooldown(game, 0) > 0) {
        return false; // On cooldown
    }
    
    if (!game->gameStarted) return false; // Need mines to be generated first
    
    // Clear previous detections
    for (int y = 0; y < game->config.height; y++) {
        for (int x = 0; x < game->config.width; x++) {
            game->board[y][x].isDetected = false;
        }
    }
    
    // Number of mines to show (1-5 based on level)
    int minesToShow = level;
    if (minesToShow < 1) minesToShow = 1;
    if (minesToShow > 5) minesToShow = 5;
    
    // Collect all unrevealed mines
    int* mineX = (int*)malloc(game->config.mines * sizeof(int));
    int* mineY = (int*)malloc(game->config.mines * sizeof(int));
    int mineCount = 0;
    
    for (int y = 0; y < game->config.height; y++) {
        for (int x = 0; x < game->config.width; x++) {
            if (game->board[y][x].hasMine && !game->board[y][x].isRevealed) {
                mineX[mineCount] = x;
                mineY[mineCount] = y;
                mineCount++;
            }
        }
    }
    
    // Show random mines
    srand((unsigned int)time(NULL));
    int shown = 0;
    bool* used = (bool*)calloc(mineCount, sizeof(bool));
    
    while (shown < minesToShow && shown < mineCount) {
        int idx = rand() % mineCount;
        if (!used[idx]) {
            game->board[mineY[idx]][mineX[idx]].isDetected = true;
            used[idx] = true;
            shown++;
        }
    }
    
    free(mineX);
    free(mineY);
    free(used);
    
    // Set cooldown
    game->detectorLastUsed = time(NULL);
    return true;
}

// Activate Shield ability - explosion protection for 3-10 seconds
// Returns true if activated, false if on cooldown
bool activateShield(GameState* game, int level) {
    // Check cooldown
    if (getAbilityCooldown(game, 1) > 0) {
        return false; // On cooldown
    }
    
    // Duration: 3-10 seconds based on level (level 1 = 3s, level 8 = 10s)
    int duration = 3 + (level - 1); // 3 + (0-7) = 3-10
    if (duration < 3) duration = 3;
    if (duration > 10) duration = 10;
    
    game->shieldActive = true;
    game->shieldStartTime = time(NULL);
    game->shieldDuration = duration;
    
    // Set cooldown
    game->shieldLastUsed = time(NULL);
    return true;
}

// Update shield status (check if expired)
void updateShield(GameState* game) {
    if (!game->shieldActive) return;
    
    time_t currentTime = time(NULL);
    int elapsed = (int)(currentTime - game->shieldStartTime);
    
    if (elapsed >= game->shieldDuration) {
        game->shieldActive = false;
    }
}

// Activate Scanner ability - shows mines in 2x2-4x4 area around cursor
// Returns true if activated, false if on cooldown
bool activateScanner(GameState* game, int level) {
    // Check cooldown
    if (getAbilityCooldown(game, 2) > 0) {
        return false; // On cooldown
    }
    
    if (!game->gameStarted) return false; // Need mines to be generated first
    
    // Clear previous scans
    for (int y = 0; y < game->config.height; y++) {
        for (int x = 0; x < game->config.width; x++) {
            game->board[y][x].isScanned = false;
        }
    }
    
    // Area size: 2x2 (level 1), 3x3 (level 2), 4x4 (level 3)
    int size = 1 + level; // 2, 3, or 4
    if (size < 2) size = 2;
    if (size > 4) size = 4;
    
    // Calculate scan area - centered on cursor
    int halfSize = (size - 1) / 2; // For 2x2: 0, for 3x3: 1, for 4x4: 1
    int startX = game->cursorX - halfSize;
    int endX = game->cursorX + (size - 1 - halfSize);
    int startY = game->cursorY - halfSize;
    int endY = game->cursorY + (size - 1 - halfSize);
    
    // Scan area around cursor
    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            if (x >= 0 && x < game->config.width && y >= 0 && y < game->config.height) {
                if (game->board[y][x].hasMine) {
                    game->board[y][x].isScanned = true;
                }
            }
        }
    }
    
    // Set cooldown
    game->scannerLastUsed = time(NULL);
    return true;
}
