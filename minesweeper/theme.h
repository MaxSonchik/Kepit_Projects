#ifndef THEME_H
#define THEME_H

#include "common.h"

// Theme structure - defines characters and colors for different game elements
typedef struct {
    const char* name;           // Theme name
    const char* mine;           // Character for mine
    const char* flag;           // Character for flag
    const char* empty;          // Character for empty cell
    const char* hidden;         // Character for hidden cell
    const char* detected;       // Character for detected mine
    const char* scanned;        // Character for scanned mine
    // Color attributes (WORD type for SetConsoleTextAttribute)
    WORD mineColor;             // Color for mine
    WORD flagColor;             // Color for flag
    WORD numberColor;           // Color for numbers (adjacent mines)
    WORD emptyColor;            // Color for empty cell
    WORD hiddenColor;          // Color for hidden cell
    WORD detectedColor;        // Color for detected mine
    WORD scannedColor;         // Color for scanned mine
    WORD cursorBgColor;        // Background color for cursor
} Theme;

// Theme definitions
extern const Theme themes[NUM_THEMES];

#endif // THEME_H
