#include "common.h"
#include "display.h"
#include "input.h"
#include "menu.h"
#include "game.h"
#include "settings.h"
#include "store.h"
#include "statistics.h"
#include "achievements.h"

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int selected = 0;
    bool running = true;
    
    // Hide cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    while (running) {
        clearScreen();
        displayMenu(selected);
        
        // Wait for key press
        int key = getKeyPress();
        
        switch (key) {
            case 'W':
            case 'w':
            case VK_UP:
                selected = (selected - 1 + MENU_ITEMS) % MENU_ITEMS;
                break;
                
            case 'S':
            case 's':
            case VK_DOWN:
                selected = (selected + 1) % MENU_ITEMS;
                break;
                
            case VK_RETURN:
            case VK_SPACE:
                // Menu item selected
                if (selected == 0) { // Start
                    GameConfig config = showDifficultyMenu();
                    Settings settings;
                    loadSettings(&settings);
                    startGame(config, settings.godMode);
                } else if (selected == 1) { // Store
                    showStore();
                } else if (selected == 2) { // Settings
                    showSettings();
                } else if (selected == 3) { // Achievements
                    showAchievements();
                } else if (selected == 4) { // Statistics
                    showStatistics();
                } else if (selected == 5) { // Exit
                    running = false;
                }
                break;
                
            case VK_ESCAPE:
                running = false;
                break;
        }
    }
    
    clearScreen();
    gotoxy(0, 0);
    printf("Thanks for playing!\n");
    
    return 0;
}
