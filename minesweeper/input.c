#include "input.h"

int getKeyPress() {
    // Wait for a key to be pressed
    while (true) {
        // Check arrow keys
        if (isKeyPressed(VK_UP)) {
            waitForKeyRelease(VK_UP);
            return VK_UP;
        }
        if (isKeyPressed(VK_DOWN)) {
            waitForKeyRelease(VK_DOWN);
            return VK_DOWN;
        }
        if (isKeyPressed(VK_LEFT)) {
            waitForKeyRelease(VK_LEFT);
            return VK_LEFT;
        }
        if (isKeyPressed(VK_RIGHT)) {
            waitForKeyRelease(VK_RIGHT);
            return VK_RIGHT;
        }
        
        // Check Enter
        if (isKeyPressed(VK_RETURN)) {
            waitForKeyRelease(VK_RETURN);
            return VK_RETURN;
        }
        
        // Check Space
        if (isKeyPressed(VK_SPACE)) {
            waitForKeyRelease(VK_SPACE);
            return VK_SPACE;
        }
        
        // Check Escape
        if (isKeyPressed(VK_ESCAPE)) {
            waitForKeyRelease(VK_ESCAPE);
            return VK_ESCAPE;
        }
        
        // Check WASD keys
        if (isKeyPressed('W') || isKeyPressed('w')) {
            waitForKeyRelease('W');
            waitForKeyRelease('w');
            return 'W';
        }
        if (isKeyPressed('S') || isKeyPressed('s')) {
            waitForKeyRelease('S');
            waitForKeyRelease('s');
            return 'S';
        }
        if (isKeyPressed('A') || isKeyPressed('a')) {
            waitForKeyRelease('A');
            waitForKeyRelease('a');
            return 'A';
        }
        if (isKeyPressed('D') || isKeyPressed('d')) {
            waitForKeyRelease('D');
            waitForKeyRelease('d');
            return 'D';
        }
        
        // Check F key for flagging
        if (isKeyPressed('F') || isKeyPressed('f')) {
            waitForKeyRelease('F');
            waitForKeyRelease('f');
            return 'F';
        }
        
        // Check number keys for abilities
        if (isKeyPressed('1')) {
            waitForKeyRelease('1');
            return '1';
        }
        if (isKeyPressed('2')) {
            waitForKeyRelease('2');
            return '2';
        }
        if (isKeyPressed('3')) {
            waitForKeyRelease('3');
            return '3';
        }
        
        // Small delay to prevent CPU spinning
        Sleep(10);
    }
}

bool isKeyPressed(int vKey) {
    return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}

void waitForKeyRelease(int vKey) {
    while (isKeyPressed(vKey)) {
        Sleep(10);
    }
}
