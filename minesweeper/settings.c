#include "settings.h"
#include "display.h"
#include "input.h"
#include "theme.h"

// Load settings from file
void loadSettings(Settings* settings) {
    FILE* file = fopen(SETTINGS_FILE, "rb");
    if (file != NULL) {
        // Check file size to handle old save files
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        // Read the old structure first (just godMode)
        bool oldGodMode = false;
        if (fileSize >= sizeof(bool)) {
            fread(&oldGodMode, sizeof(bool), 1, file);
        }
        
        // If file is old format, initialize with defaults
        if (fileSize < (long)sizeof(Settings)) {
            settings->godMode = oldGodMode;
            settings->modifiersEnabled = false;
            settings->themeIndex = 0;  // Default theme
        } else {
            // New format - read full structure
            fseek(file, 0, SEEK_SET);
            fread(settings, sizeof(Settings), 1, file);
            // Validate theme index
            if (settings->themeIndex < 0 || settings->themeIndex >= NUM_THEMES) {
                settings->themeIndex = 0;
            }
        }
        fclose(file);
    } else {
        // Initialize with default values
        settings->godMode = false;
        settings->modifiersEnabled = false;
        settings->themeIndex = 0;  // Default theme
    }
}

// Save settings to file
void saveSettings(Settings* settings) {
    FILE* file = fopen(SETTINGS_FILE, "wb");
    if (file != NULL) {
        fwrite(settings, sizeof(Settings), 1, file);
        fclose(file);
    }
}

// Display settings screen
void showSettings() {
    Settings settings;
    loadSettings(&settings);
    
    int selected = 0; // 0 = God Mode, 1 = Modifiers, 2 = Theme
    bool viewing = true;
    
    while (viewing) {
        clearScreen();
        
        // Header
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        gotoxy(0, 2);
        printf("========================================\n");
        printf("          SETTINGS\n");
        printf("========================================\n\n");
        
        // God Mode option
        gotoxy(0, 6);
        if (selected == 0) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("  > God Mode: ");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("    God Mode: ");
        }
        
        if (settings.godMode) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("ON");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("OFF");
        }
        if (selected == 0) {
            printf(" <\n");
        } else {
            printf("\n");
        }
        
        // Modifiers option
        gotoxy(0, 7);
        if (selected == 1) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("  > Modifiers: ");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("    Modifiers: ");
        }
        
        if (settings.modifiersEnabled) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("ON");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("OFF");
        }
        if (selected == 1) {
            printf(" <\n");
        } else {
            printf("\n");
        }
        
        // Theme option
        gotoxy(0, 8);
        if (selected == 2) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("  > Theme: ");
        } else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("    Theme: ");
        }
        
        // Validate theme index
        if (settings.themeIndex < 0 || settings.themeIndex >= NUM_THEMES) {
            settings.themeIndex = 0;
        }
        
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("%s", themes[settings.themeIndex].name);
        if (selected == 2) {
            printf(" <\n");
        } else {
            printf("\n");
        }
        
        // Description
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        gotoxy(0, 10);
        if (selected == 0) {
            printf("\nWhen God Mode is ON:\n");
            printf("  - Defeats do not count in statistics\n");
            printf("  - No progress in achievements\n");
            printf("  - You can still hit mines and lose\n");
        } else if (selected == 1) {
            printf("\nWhen Modifiers are ON:\n");
            printf("  - 1 random modifier will appear in each game\n");
            printf("  - Modifiers: Random Ability, Disable Abilities,\n");
            printf("    or Disable Timer Display\n");
        } else if (selected == 2) {
            const Theme* previewTheme = &themes[settings.themeIndex];
            printf("\nTheme Preview:\n");
            printf("  Characters: Mine: %s  Flag: %s  Empty: %s  Hidden: %s\n", 
                   previewTheme->mine,
                   previewTheme->flag,
                   previewTheme->empty,
                   previewTheme->hidden);
            printf("  Detected: %s  Scanned: %s\n",
                   previewTheme->detected,
                   previewTheme->scanned);
            printf("  Colors: ");
            SetConsoleTextAttribute(hConsole, previewTheme->mineColor);
            printf("Mine ");
            SetConsoleTextAttribute(hConsole, previewTheme->flagColor);
            printf("Flag ");
            SetConsoleTextAttribute(hConsole, previewTheme->numberColor);
            printf("Numbers ");
            SetConsoleTextAttribute(hConsole, previewTheme->emptyColor);
            printf("Empty ");
            SetConsoleTextAttribute(hConsole, previewTheme->hiddenColor);
            printf("Hidden");
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("\n\nUse LEFT/RIGHT to change theme\n");
        }
        
        // Reset text color
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        
        gotoxy(0, 18);
        printf("\nUse UP/DOWN to navigate\n");
        if (selected == 2) {
            printf("Use LEFT/RIGHT to change theme\n");
        } else {
            printf("Press ENTER/SPACE to toggle, ESC to return\n");
        }
        
        int key = getKeyPress();
        
        switch (key) {
            case 'W':
            case 'w':
            case VK_UP:
                selected = (selected - 1 + 3) % 3;
                break;
                
            case 'S':
            case 's':
            case VK_DOWN:
                selected = (selected + 1) % 3;
                break;
                
            case VK_LEFT:
            case 'A':
            case 'a':
                if (selected == 2) {
                    settings.themeIndex = (settings.themeIndex - 1 + NUM_THEMES) % NUM_THEMES;
                    saveSettings(&settings);
                }
                break;
                
            case VK_RIGHT:
            case 'D':
            case 'd':
                if (selected == 2) {
                    settings.themeIndex = (settings.themeIndex + 1) % NUM_THEMES;
                    saveSettings(&settings);
                }
                break;
                
            case VK_RETURN:
            case VK_SPACE:
                if (selected == 0) {
                    settings.godMode = !settings.godMode;
                    saveSettings(&settings);
                } else if (selected == 1) {
                    settings.modifiersEnabled = !settings.modifiersEnabled;
                    saveSettings(&settings);
                }
                break;
                
            case VK_ESCAPE:
                viewing = false;
                break;
        }
    }
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
