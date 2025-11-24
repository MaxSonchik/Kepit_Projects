#include "input.h"
#include <ncurses.h>

InputCode get_input() {
    int ch = getch();
    
    switch (ch) {
        case 'q':
        case 'Q':
            return INPUT_QUIT;
        case KEY_UP:
        case 'w':
        case 'W':
            return INPUT_UP;
        case KEY_DOWN:
        case 's':
        case 'S':
            return INPUT_DOWN;
        case KEY_LEFT:
        case 'a':
        case 'A':
            return INPUT_LEFT;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            return INPUT_RIGHT;
        case '\n':
        case ' ':
            return INPUT_SELECT;
        case 'f':
        case 'F':
            return INPUT_FLAG;
        case KEY_BACKSPACE:
        case 27: // ESC
            return INPUT_BACK;
        default:
            return INPUT_NONE;
    }
}