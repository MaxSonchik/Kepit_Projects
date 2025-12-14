#include "menu.h"
#include "display.h"
#include "input.h"

// Menu options
const char* menu_items[MENU_ITEMS] = {
    "Start",
    "Store",
    "Settings",
    "Achievements",
    "Statistics",
    "Exit"
};

// Difficulty options
const char* difficulty_items[DIFFICULTY_ITEMS] = {
    "Easy",
    "Medium",
    "Complex",
    "Custom"
};

void displayMenu(int selected) {
    gotoxy(0, 2);
    printf("========================================\n");
    printf("          MINESWEEPER GAME\n");
    printf("========================================\n\n");
    
    for (int i = 0; i < MENU_ITEMS; i++) {
        gotoxy(0, 5 + i);
        
        if (i == selected) {
            // Highlight selected item
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("  > %d. %s <\n", i + 1, menu_items[i]);
        } else {
            // Normal item
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("    %d. %s\n", i + 1, menu_items[i]);
        }
    }
    
    // Reset text color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    gotoxy(0, 12);
    printf("\nUse ARROW KEYS or WASD to navigate\n");
    printf("Press ENTER to select, ESC to exit\n");
}

GameConfig showDifficultyMenu() {
    int selected = 0;
    GameConfig config = PRESET_EASY;
    bool menuActive = true;
    
    while (menuActive) {
        clearScreen();
        displayDifficultyMenu(selected, &config);
        
        int key = getKeyPress();
        
        switch (key) {
            case 'W':
            case 'w':
            case VK_UP:
                selected = (selected - 1 + DIFFICULTY_ITEMS) % DIFFICULTY_ITEMS;
                break;
                
            case 'S':
            case 's':
            case VK_DOWN:
                selected = (selected + 1) % DIFFICULTY_ITEMS;
                break;
                
            case VK_RETURN:
            case VK_SPACE:
                if (selected == 0) { // Easy
                    return PRESET_EASY;
                } else if (selected == 1) { // Medium
                    return PRESET_MEDIUM;
                } else if (selected == 2) { // Complex
                    return PRESET_COMPLEX;
                } else if (selected == 3) { // Custom
                    return configureCustomDifficulty();
                }
                break;
                
            case VK_ESCAPE:
                return PRESET_EASY; // Return default on escape
        }
    }
    
    return config;
}

void displayDifficultyMenu(int selected, GameConfig* config) {
    gotoxy(0, 2);
    printf("========================================\n");
    printf("      SELECT DIFFICULTY\n");
    printf("========================================\n\n");
    
    for (int i = 0; i < DIFFICULTY_ITEMS; i++) {
        gotoxy(0, 5 + i);
        
        if (i == selected) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("  > %s <\n", difficulty_items[i]);
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("    %s\n", difficulty_items[i]);
        }
    }
    
    // Reset text color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    // Show preset info
    gotoxy(0, 10);
    if (selected == 0) {
        printf("\nEasy: %dx%d field, %d mines\n", PRESET_EASY.width, PRESET_EASY.height, PRESET_EASY.mines);
    } else if (selected == 1) {
        printf("\nMedium: %dx%d field, %d mines\n", PRESET_MEDIUM.width, PRESET_MEDIUM.height, PRESET_MEDIUM.mines);
    } else if (selected == 2) {
        printf("\nComplex: %dx%d field, %d mines\n", PRESET_COMPLEX.width, PRESET_COMPLEX.height, PRESET_COMPLEX.mines);
    } else if (selected == 3) {
        printf("\nCustom: Set your own field size and mine count\n");
    }
    
    gotoxy(0, 13);
    printf("\nUse ARROW KEYS or WASD to navigate\n");
    printf("Press ENTER to select, ESC to go back\n");
}

GameConfig configureCustomDifficulty() {
    GameConfig config = {10, 10, 10}; // Default custom values
    int selectedField = 0; // 0 = width, 1 = height, 2 = mines
    bool configuring = true;
    
    while (configuring) {
        clearScreen();
        displayCustomDifficulty(&config, selectedField);
        
        int key = getKeyPress();
        
        switch (key) {
            case 'W':
            case 'w':
            case VK_UP:
                selectedField = (selectedField - 1 + 3) % 3;
                break;
                
            case 'S':
            case 's':
            case VK_DOWN:
                selectedField = (selectedField + 1) % 3;
                break;
                
            case VK_LEFT:
            case 'A':
            case 'a':
                // Decrease selected value
                if (selectedField == 0 && config.width > 5) {
                    config.width--;
                } else if (selectedField == 1 && config.height > 5) {
                    config.height--;
                } else if (selectedField == 2 && config.mines > 1) {
                    config.mines--;
                }
                break;
                
            case VK_RIGHT:
            case 'D':
            case 'd':
                // Increase selected value
                if (selectedField == 0 && config.width < 50) {
                    config.width++;
                } else if (selectedField == 1 && config.height < 50) {
                    config.height++;
                } else if (selectedField == 2 && config.mines < config.width * config.height - 1) {
                    config.mines++;
                }
                break;
                
            case VK_RETURN:
            case VK_SPACE:
                // Validate and return
                if (config.mines >= config.width * config.height) {
                    config.mines = config.width * config.height - 1;
                }
                if (config.mines < 1) {
                    config.mines = 1;
                }
                return config;
                
            case VK_ESCAPE:
                return PRESET_EASY; // Return default on escape
        }
    }
    
    return config;
}

void displayCustomDifficulty(GameConfig* config, int selectedField) {
    gotoxy(0, 2);
    printf("========================================\n");
    printf("      CUSTOM DIFFICULTY\n");
    printf("========================================\n\n");
    
    // Width
    gotoxy(0, 6);
    if (selectedField == 0) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("  > Width:  ");
    } else {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("    Width:  ");
    }
    printf("%d", config->width);
    if (selectedField == 0) {
        printf(" <\n");
    } else {
        printf("\n");
    }
    
    // Height
    gotoxy(0, 7);
    if (selectedField == 1) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("  > Height: ");
    } else {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("    Height: ");
    }
    printf("%d", config->height);
    if (selectedField == 1) {
        printf(" <\n");
    } else {
        printf("\n");
    }
    
    // Mines
    gotoxy(0, 8);
    if (selectedField == 2) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("  > Mines:  ");
    } else {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("    Mines:  ");
    }
    printf("%d", config->mines);
    if (selectedField == 2) {
        printf(" <\n");
    } else {
        printf("\n");
    }
    
    // Reset text color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    gotoxy(0, 11);
    printf("\nUse UP/DOWN to select field\n");
    printf("Use LEFT/RIGHT or A/D to adjust value\n");
    printf("Press ENTER to confirm, ESC to cancel\n");
    
    // Show field info
    gotoxy(0, 15);
    printf("Field size: %dx%d = %d cells\n", config->width, config->height, config->width * config->height);
    printf("Maximum mines: %d\n", config->width * config->height - 1);
}
