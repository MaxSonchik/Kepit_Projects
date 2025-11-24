#include <stdio.h>
#include "../core/game.h"

void print_field() {
    printf("\n   ");
    for (int i = 1; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d   ", i + 1);
        for (int j = 0; j < SIZE; j++){
            printf("%c ", visible[i][j]);
        }
        printf("\n");
    }
    printf("Мин осталось: %d\n", mines_left);
}

void print_game_over() {
    printf("ФИАСКО! Ты подорвался на мине/n");

    printf("Расположение мин:\n");
    printf("    ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d", i);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            if (field[i][j] == 'X') {
                printf("X ");
            } else {
                printf("%c ", visible[i][j]);
            }
        }
        printf("\n");
    }
}

void print_victory() {
    printf("ПОБЕДА! Ты выйграл\n");
}