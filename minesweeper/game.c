#include "game.h"
#include "display.h"
#include "input.h"
#include "abilities.h"
#include "statistics.h"
#include "achievements.h"
#include "store.h"
#include "settings.h"

// Initialize game state
void initializeGame(GameState* game) {
    // Allocate board
    game->board = (Cell**)malloc(game->config.height * sizeof(Cell*));
    for (int i = 0; i < game->config.height; i++) {
        game->board[i] = (Cell*)malloc(game->config.width * sizeof(Cell));
        for (int j = 0; j < game->config.width; j++) {
            game->board[i][j].hasMine = false;
            game->board[i][j].isRevealed = false;
            game->board[i][j].isFlagged = false;
            game->board[i][j].adjacentMines = 0;
            game->board[i][j].isDetected = false;
            game->board[i][j].isScanned = false;
        }
    }
    
    game->cursorX = 0;
    game->cursorY = 0;
    game->gameStarted = false;
    game->gameOver = false;
    game->gameWon = false;
    game->startTime = 0;
    game->elapsedSeconds = 0;
    game->flagsPlaced = 0;
    game->cellsRevealed = 0;
    game->shieldActive = false;
    game->shieldStartTime = 0;
    game->shieldDuration = 0;
    game->detectorLastUsed = 0;
    game->shieldLastUsed = 0;
    game->scannerLastUsed = 0;
    game->activeModifier = MODIFIER_NONE;
    game->lastRandomAbilityTime = 0;
}

// Generate mines after first move (ensuring first move is safe)
void generateMines(GameState* game, int firstX, int firstY) {
    srand((unsigned int)time(NULL));
    int minesPlaced = 0;
    int totalCells = game->config.width * game->config.height;
    
    while (minesPlaced < game->config.mines) {
        int x = rand() % game->config.width;
        int y = rand() % game->config.height;
        
        // Don't place mine at first click position or adjacent cells
        if (x == firstX && y == firstY) continue;
        if (abs(x - firstX) <= 1 && abs(y - firstY) <= 1) continue;
        
        // Don't place mine if there's already one
        if (!game->board[y][x].hasMine) {
            game->board[y][x].hasMine = true;
            minesPlaced++;
        }
    }
    
    // Calculate adjacent mine counts
    for (int y = 0; y < game->config.height; y++) {
        for (int x = 0; x < game->config.width; x++) {
            if (!game->board[y][x].hasMine) {
                game->board[y][x].adjacentMines = countAdjacentMines(game, x, y);
            }
        }
    }
}

// Count adjacent mines
int countAdjacentMines(GameState* game, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < game->config.width && ny >= 0 && ny < game->config.height) {
                if (game->board[ny][nx].hasMine) {
                    count++;
                }
            }
        }
    }
    return count;
}

// Reveal a cell
void revealCell(GameState* game, int x, int y, bool godMode) {
    if (x < 0 || x >= game->config.width || y < 0 || y >= game->config.height) return;
    if (game->board[y][x].isRevealed || game->board[y][x].isFlagged) return;
    
    // Generate mines on first move
    if (!game->gameStarted) {
        generateMines(game, x, y);
        game->gameStarted = true;
        game->startTime = time(NULL);
    }
    
    game->board[y][x].isRevealed = true;
    game->cellsRevealed++;
    
    // Check if hit a mine
    if (game->board[y][x].hasMine) {
        // Check for shield protection
        updateShield(game);
        if (game->shieldActive) {
            // Shield protects from explosion - don't end game, just reveal the mine
            return;
        }
        // In god mode, hitting a mine doesn't end the game
        if (!godMode) {
            game->gameOver = true;
        }
        return;
    }
    
    // If cell has no adjacent mines, reveal adjacent cells
    if (game->board[y][x].adjacentMines == 0) {
        revealAdjacentCells(game, x, y, godMode);
    }
}

// Reveal adjacent cells (for empty cells)
void revealAdjacentCells(GameState* game, int x, int y, bool godMode) {
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < game->config.width && ny >= 0 && ny < game->config.height) {
                if (!game->board[ny][nx].isRevealed && !game->board[ny][nx].isFlagged) {
                    revealCell(game, nx, ny, godMode);
                }
            }
        }
    }
}

// Toggle flag
void toggleFlag(GameState* game, int x, int y) {
    if (x < 0 || x >= game->config.width || y < 0 || y >= game->config.height) return;
    if (game->board[y][x].isRevealed) return;
    
    if (game->board[y][x].isFlagged) {
        game->board[y][x].isFlagged = false;
        game->flagsPlaced--;
    } else {
        game->board[y][x].isFlagged = true;
        game->flagsPlaced++;
    }
}

// Check if game is won
bool checkWin(GameState* game) {
    int totalCells = game->config.width * game->config.height;
    return game->cellsRevealed == (totalCells - game->config.mines);
}

// Cleanup game resources
void cleanupGame(GameState* game) {
    if (game->board) {
        for (int i = 0; i < game->config.height; i++) {
            free(game->board[i]);
        }
        free(game->board);
        game->board = NULL;
    }
}

// Main game loop
void startGame(GameConfig config, bool godMode) {
    GameState game;
    game.config = config;
    
    // Load store to check for abilities
    Store store;
    loadStore(&store);
    
    // Load settings to check for modifiers
    Settings settings;
    loadSettings(&settings);
    
    initializeGame(&game);
    
    // Randomly select a modifier if enabled
    if (settings.modifiersEnabled) {
        srand((unsigned int)time(NULL));
        int modifierChoice = (rand() % 3) + 1; // 1, 2, or 3
        game.activeModifier = (ModifierType)modifierChoice;
    }
    
    bool gameRunning = true;
    bool gameCompleted = false; // Track if game was completed (won/lost) vs exited
    
    while (gameRunning) {
        displayGame(&game, godMode, store.hasDetector, store.hasShield, store.hasScanner);
        
        // Handle Random Ability modifier
        if (game.activeModifier == MODIFIER_RANDOM_ABILITY && game.gameStarted && !game.gameOver && !game.gameWon) {
            time_t currentTime = time(NULL);
            // Trigger random ability every 20 seconds (or immediately if never triggered)
            int timeSinceLastAbility = (game.lastRandomAbilityTime == 0) ? 999 : (int)(currentTime - game.lastRandomAbilityTime);
            
            if (timeSinceLastAbility >= 20) {
                // Randomly select one of the three abilities (regardless of ownership)
                int abilityChoice = rand() % 3;
                bool activated = false;
                const char* abilityName = "";
                int abilityLevel = 1; // Default to level 1 if not owned
                
                if (abilityChoice == 0) {
                    // Detector
                    abilityName = "Detector";
                    if (store.hasDetector) {
                        abilityLevel = store.detectorLevel;
                    }
                    // Only activate if not on cooldown
                    if (getAbilityCooldown(&game, 0) == 0) {
                        activated = activateDetector(&game, abilityLevel);
                    }
                } else if (abilityChoice == 1) {
                    // Shield
                    abilityName = "Shield";
                    if (store.hasShield) {
                        abilityLevel = store.shieldLevel;
                    }
                    // Only activate if not on cooldown
                    if (getAbilityCooldown(&game, 1) == 0) {
                        activated = activateShield(&game, abilityLevel);
                    }
                } else if (abilityChoice == 2) {
                    // Scanner
                    abilityName = "Scanner";
                    if (store.hasScanner) {
                        abilityLevel = store.scannerLevel;
                    }
                    // Only activate if not on cooldown
                    if (getAbilityCooldown(&game, 2) == 0) {
                        activated = activateScanner(&game, abilityLevel);
                    }
                }
                
                if (activated) {
                    game.lastRandomAbilityTime = currentTime;
                    // Show brief message that ability was randomly activated
                    gotoxy(0, game.config.height + 20);
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    printf("Random Ability Modifier: %s activated!", abilityName);
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    Sleep(1500); // Show message for 1.5 seconds
                } else {
                    // Ability was on cooldown, try again in 5 seconds
                    game.lastRandomAbilityTime = currentTime - 15; // Will trigger again in 5 seconds
                }
            }
        }
        
        // Check win condition
        if (!game.gameOver && checkWin(&game)) {
            // Calculate and award coins
            Statistics stats;
            loadStatistics(&stats);
            int coinsEarned = calculateGameCoins(config, true, game.elapsedSeconds, game.flagsPlaced, &stats, godMode);
            if (coinsEarned > 0) {
                awardCoins(coinsEarned);
            }
            showGameOver(&game, true, godMode, store.hasDetector, store.hasShield, store.hasScanner, coinsEarned);
            gameCompleted = true;
            getKeyPress();
            break;
        }
        
        // Check time limit
        if (MAX_TIME != 999 && game.gameStarted && !game.gameOver && game.elapsedSeconds >= MAX_TIME) {
            showGameOver(&game, false, godMode, store.hasDetector, store.hasShield, store.hasScanner, 0);
            gameCompleted = true;
            getKeyPress();
            break;
        }
        
        if (game.gameOver) {
            gameCompleted = true;
            break;
        }
        
        // Get input
        int key = getKeyPress();
        
        switch (key) {
            case VK_UP:
            case 'W':
            case 'w':
                if (game.cursorY > 0) game.cursorY--;
                break;
                
            case VK_DOWN:
            case 'S':
            case 's':
                if (game.cursorY < game.config.height - 1) game.cursorY++;
                break;
                
            case VK_LEFT:
            case 'A':
            case 'a':
                if (game.cursorX > 0) game.cursorX--;
                break;
                
            case VK_RIGHT:
            case 'D':
            case 'd':
                if (game.cursorX < game.config.width - 1) game.cursorX++;
                break;
                
            case VK_RETURN:
            case VK_SPACE:
                revealCell(&game, game.cursorX, game.cursorY, godMode);
                if (game.gameOver) {
                    showGameOver(&game, false, godMode, store.hasDetector, store.hasShield, store.hasScanner, 0);
                    gameCompleted = true;
                    getKeyPress();
                    gameRunning = false;
                }
                break;
                
            case 'F':
            case 'f':
                toggleFlag(&game, game.cursorX, game.cursorY);
                break;
                
            case '1':
                // Activate Detector
                if (game.activeModifier == MODIFIER_DISABLE_ABILITIES) {
                    gotoxy(0, game.config.height + 25);
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("Abilities are disabled by modifier!");
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    Sleep(1000);
                } else if (store.hasDetector) {
                    if (!game.gameStarted) {
                        // Show message that game needs to start first
                        gotoxy(0, game.config.height + 25);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        printf("Detector requires game to be started first!");
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        Sleep(1000);
                    } else if (!activateDetector(&game, store.detectorLevel)) {
                        // On cooldown
                        int cooldown = getAbilityCooldown(&game, 0);
                        gotoxy(0, game.config.height + 25);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        printf("Detector on cooldown! %d seconds remaining.", cooldown);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        Sleep(1000);
                    }
                }
                break;
                
            case '2':
                // Activate Shield
                if (game.activeModifier == MODIFIER_DISABLE_ABILITIES) {
                    gotoxy(0, game.config.height + 25);
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("Abilities are disabled by modifier!");
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    Sleep(1000);
                } else if (store.hasShield) {
                    if (!activateShield(&game, store.shieldLevel)) {
                        // On cooldown
                        int cooldown = getAbilityCooldown(&game, 1);
                        gotoxy(0, game.config.height + 25);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        printf("Shield on cooldown! %d seconds remaining.", cooldown);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        Sleep(1000);
                    }
                }
                break;
                
            case '3':
                // Activate Scanner
                if (game.activeModifier == MODIFIER_DISABLE_ABILITIES) {
                    gotoxy(0, game.config.height + 25);
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    printf("Abilities are disabled by modifier!");
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    Sleep(1000);
                } else if (store.hasScanner) {
                    if (!game.gameStarted) {
                        // Show message that game needs to start first
                        gotoxy(0, game.config.height + 25);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        printf("Scanner requires game to be started first!");
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        Sleep(1000);
                    } else if (!activateScanner(&game, store.scannerLevel)) {
                        // On cooldown
                        int cooldown = getAbilityCooldown(&game, 2);
                        gotoxy(0, game.config.height + 25);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        printf("Scanner on cooldown! %d seconds remaining.", cooldown);
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        Sleep(1000);
                    }
                }
                break;
                
            case VK_ESCAPE:
                gameRunning = false;
                break;
        }
    }
    
    // Update statistics if game was completed (not just exited)
    if (gameCompleted && game.gameStarted) {
        Statistics stats;
        loadStatistics(&stats);
        updateStatistics(&stats, config, game.gameWon, game.elapsedSeconds, game.flagsPlaced, godMode);
        
        // Check achievements (only if god mode is off)
        Achievements achievements;
        loadAchievements(&achievements);
        checkAchievements(&achievements, &stats, config, game.gameWon, game.elapsedSeconds, game.flagsPlaced, godMode);
    }
    
    cleanupGame(&game);
}
