#include "theme.h"

// Theme definitions
const Theme themes[NUM_THEMES] = {
    // Default theme - classic colors
    {"Default", "*", "F", ".", "#", "D", "S",
     FOREGROUND_RED | FOREGROUND_INTENSITY,                    // mineColor
     FOREGROUND_RED | FOREGROUND_INTENSITY,                     // flagColor
     FOREGROUND_GREEN | FOREGROUND_INTENSITY,                   // numberColor
     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,      // emptyColor
     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,      // hiddenColor
     FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // detectedColor
     FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,  // scannedColor
     BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY}, // cursorBgColor
    
    // Modern theme - same colors, different characters
    {"Modern", "X", "!", " ", "?", "!", "?",
     FOREGROUND_RED | FOREGROUND_INTENSITY,
     FOREGROUND_RED | FOREGROUND_INTENSITY,
     FOREGROUND_GREEN | FOREGROUND_INTENSITY,
     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
     FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
     FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
     BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY},
    
    // Minimal theme - same colors, different characters
    {"Minimal", "M", "P", " ", "·", "!", "?",
     FOREGROUND_RED | FOREGROUND_INTENSITY,
     FOREGROUND_RED | FOREGROUND_INTENSITY,
     FOREGROUND_GREEN | FOREGROUND_INTENSITY,
     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
     FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
     FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
     BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY},
    
    // Color theme - different color scheme (blue/purple theme)
    {"Color", "*", "F", ".", "#", "D", "S",
     FOREGROUND_BLUE | FOREGROUND_INTENSITY,                    // mineColor - blue
     FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,   // flagColor - magenta
     FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // numberColor - cyan
     FOREGROUND_BLUE | FOREGROUND_GREEN,                        // emptyColor - light blue
     FOREGROUND_BLUE,                                            // hiddenColor - dark blue
     FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,  // detectedColor - yellow
     FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,   // scannedColor - magenta
     BACKGROUND_BLUE | BACKGROUND_INTENSITY}                     // cursorBgColor - bright blue
};
