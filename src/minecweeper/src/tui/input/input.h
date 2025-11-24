#ifndef INPUT_H
#define INPUT_H

typedef enum {
    INPUT_UP,
    INPUT_DOWN, 
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_SELECT,
    INPUT_FLAG,
    INPUT_BACK,
    INPUT_QUIT,
    INPUT_NONE
} InputCode;

InputCode get_input();

#endif