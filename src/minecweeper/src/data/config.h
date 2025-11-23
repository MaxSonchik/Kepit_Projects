#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int size;
    int mines;
} GameConfig;

GameConfig get_beginner_config();

#endif