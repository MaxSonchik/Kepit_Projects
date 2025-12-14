#include "display.h"
#include "theme.h"
#include "settings.h"
#include "abilities.h"
#include "store.h"

void gotoxy(int x, int y) {
    cursorPos.X = x;
    cursorPos.Y = y;
    SetConsoleCursorPosition(hConsole, cursorPos);
}

void clearScreen() {
    system("cls");
}

// Display the game board
void displayGame(GameState* game, bool godMode, bool hasDetector, bool hasShield, bool hasScanner) {
    clearScreen();
    
    // Load current theme
    Settings settings;
    loadSettings(&settings);
    if (settings.themeIndex < 0 || settings.themeIndex >= NUM_THEMES) {
        settings.themeIndex = 0;
    }
    const Theme* theme = &themes[settings.themeIndex];
    
    // Update timer
    if (game->gameStarted && !game->gameOver && !game->gameWon) {
        time_t currentTime = time(NULL);
        game->elapsedSeconds = (int)(currentTime - game->startTime);
    }
    
    // Display header
    gotoxy(0, 0);
    printf("========================================\n");
    printf("          MINESWEEPER GAME\n");
    printf("========================================\n");
    
    // Update shield status
    updateShield(game);
    
    // Display game info
    gotoxy(0, 4);
    printf("Mines: %d/%d  |  Flags: %d", 
           game->config.mines, game->config.mines, game->flagsPlaced);
    
    // Show timer only if not disabled by modifier
    if (game->activeModifier != MODIFIER_DISABLE_TIMER) {
        printf("  |  Time: %03d", game->elapsedSeconds);
        if (MAX_TIME != 999 && game->elapsedSeconds >= MAX_TIME) {
            printf(" (TIME UP!)");
        }
    }
    if (godMode) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("  |  GOD MODE: ON");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    if (game->shieldActive) {
        time_t currentTime = time(NULL);
        int remaining = game->shieldDuration - (int)(currentTime - game->shieldStartTime);
        if (remaining < 0) remaining = 0;
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("  |  SHIELD: %ds", remaining);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    
    // Display active modifier
    if (game->activeModifier != MODIFIER_NONE) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("  |  MODIFIER: ");
        switch (game->activeModifier) {
            case MODIFIER_RANDOM_ABILITY:
                printf("Random Ability");
                break;
            case MODIFIER_DISABLE_ABILITIES:
                printf("Abilities Disabled");
                break;
            case MODIFIER_DISABLE_TIMER:
                printf("Timer Hidden");
                break;
            default:
                break;
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    printf("\n");
    
    // Display cursor position
    gotoxy(0, 5);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("Cursor Position: X=%d, Y=%d", game->cursorX, game->cursorY);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\n");
    
    // Display board
    gotoxy(0, 7);
    printf("   ");
    for (int x = 0; x < game->config.width; x++) {
        if (x == game->cursorX) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("%2d", x);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else {
            printf("%2d", x);
        }
    }
    printf("\n");
    
    for (int y = 0; y < game->config.height; y++) {
        if (y == game->cursorY) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("%2d ", y);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else {
            printf("%2d ", y);
        }
        
        for (int x = 0; x < game->config.width; x++) {
            Cell* cell = &game->board[y][x];
            bool isCursor = (x == game->cursorX && y == game->cursorY);
            
            // Highlight cursor with theme background
            if (isCursor) {
                SetConsoleTextAttribute(hConsole, theme->cursorBgColor);
                printf("[");
            } else {
                printf(" ");
            }
            
            // Display cell content
            if (game->gameOver && cell->hasMine) {
                // Show all mines when game is over
                if (isCursor) {
                    SetConsoleTextAttribute(hConsole, theme->mineColor | theme->cursorBgColor);
                } else {
                    SetConsoleTextAttribute(hConsole, theme->mineColor);
                }
                printf("%s", theme->mine);
            } else if (cell->isFlagged) {
                if (isCursor) {
                    SetConsoleTextAttribute(hConsole, theme->flagColor | theme->cursorBgColor);
                } else {
                    SetConsoleTextAttribute(hConsole, theme->flagColor);
                }
                printf("%s", theme->flag);
            } else if (cell->isRevealed) {
                if (cell->hasMine) {
                    if (isCursor) {
                        SetConsoleTextAttribute(hConsole, theme->mineColor | theme->cursorBgColor);
                    } else {
                        SetConsoleTextAttribute(hConsole, theme->mineColor);
                    }
                    printf("%s", theme->mine);
                } else if (cell->adjacentMines > 0) {
                    if (isCursor) {
                        SetConsoleTextAttribute(hConsole, theme->numberColor | theme->cursorBgColor);
                    } else {
                        SetConsoleTextAttribute(hConsole, theme->numberColor);
                    }
                    printf("%d", cell->adjacentMines);
                } else {
                    if (isCursor) {
                        SetConsoleTextAttribute(hConsole, theme->emptyColor | theme->cursorBgColor);
                    } else {
                        SetConsoleTextAttribute(hConsole, theme->emptyColor);
                    }
                    printf("%s", theme->empty);
                }
            } else if (cell->isDetected || cell->isScanned) {
                // Show detected/scanned mines
                if (cell->hasMine) {
                    if (cell->isDetected) {
                        // Detector shows mines
                        if (isCursor) {
                            SetConsoleTextAttribute(hConsole, theme->detectedColor | theme->cursorBgColor);
                        } else {
                            SetConsoleTextAttribute(hConsole, theme->detectedColor);
                        }
                        printf("%s", theme->detected);
                    } else if (cell->isScanned) {
                        // Scanner shows mines
                        if (isCursor) {
                            SetConsoleTextAttribute(hConsole, theme->scannedColor | theme->cursorBgColor);
                        } else {
                            SetConsoleTextAttribute(hConsole, theme->scannedColor);
                        }
                        printf("%s", theme->scanned);
                    }
                } else {
                    // Not a mine, show as normal
                    if (isCursor) {
                        SetConsoleTextAttribute(hConsole, theme->hiddenColor | theme->cursorBgColor);
                    } else {
                        SetConsoleTextAttribute(hConsole, theme->hiddenColor);
                    }
                    printf("%s", theme->hidden);
                }
            } else {
                if (isCursor) {
                    SetConsoleTextAttribute(hConsole, theme->hiddenColor | theme->cursorBgColor);
                } else {
                    SetConsoleTextAttribute(hConsole, theme->hiddenColor);
                }
                printf("%s", theme->hidden);
            }
            
            // Close cursor bracket
            if (isCursor) {
                SetConsoleTextAttribute(hConsole, theme->cursorBgColor);
                printf("]");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                printf(" ");
            }
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("\n");
    }
    
    // Reset text color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    // Display controls
    gotoxy(0, game->config.height + 9);
    printf("\nControls:\n");
    printf("Arrow Keys / WASD: Move cursor\n");
    printf("ENTER / SPACE: Reveal cell\n");
    printf("F: Toggle flag\n");
    if (hasDetector) {
        int cooldown = getAbilityCooldown(game, 0);
        if (game->activeModifier == MODIFIER_DISABLE_ABILITIES) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("1: Detector [DISABLED BY MODIFIER]\n");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("1: Detector (shows random mines)");
            if (cooldown > 0) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf(" [Cooldown: %ds]", cooldown);
            }
            printf("\n");
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    if (hasShield) {
        int cooldown = getAbilityCooldown(game, 1);
        if (game->activeModifier == MODIFIER_DISABLE_ABILITIES) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("2: Shield [DISABLED BY MODIFIER]\n");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("2: Shield (explosion protection)");
            if (cooldown > 0) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf(" [Cooldown: %ds]", cooldown);
            }
            printf("\n");
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    if (hasScanner) {
        int cooldown = getAbilityCooldown(game, 2);
        if (game->activeModifier == MODIFIER_DISABLE_ABILITIES) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("3: Scanner [DISABLED BY MODIFIER]\n");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("3: Scanner (shows mines in area)");
            if (cooldown > 0) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf(" [Cooldown: %ds]", cooldown);
            }
            printf("\n");
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    printf("ESC: Return to menu\n");
    
    if (game->gameOver || game->gameWon) {
        gotoxy(0, game->config.height + 14);
        if (game->gameWon) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("YOU WON! Press any key to continue...\n");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("GAME OVER! Press any key to continue...\n");
        }
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}

// Show game over screen
void showGameOver(GameState* game, bool won, bool godMode, bool hasDetector, bool hasShield, bool hasScanner, int coinsEarned) {
    game->gameWon = won;
    game->gameOver = true;
    displayGame(game, godMode, hasDetector, hasShield, hasScanner);
    
    // Display coin earnings if won
    if (won && coinsEarned > 0) {
        gotoxy(0, game->config.height + 16);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("========================================\n");
        printf("          COINS EARNED!\n");
        printf("========================================\n");
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("Total: +%d coins\n", coinsEarned);
        
        // Show current balance
        Store store;
        loadStore(&store);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("Current Balance: %d coins\n", store.coins);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
}
