#ifndef INPUT_H
#define INPUT_H

#include "common.h"

// Input handling functions
int getKeyPress();
bool isKeyPressed(int vKey);
void waitForKeyRelease(int vKey);

#endif // INPUT_H
