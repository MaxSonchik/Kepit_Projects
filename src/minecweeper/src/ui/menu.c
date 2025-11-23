#include <stdio.h>

void show_main_menu() {
    printf("=== САПЁР ===\n");
    printf("1. Новая игра\n");
    printf("2. Выход\n");
    printf("Выберите: ");
}

int get_user_choice() {
    int choice;
    scanf("%d", &choice);
    return choice;
}

void show_game_menu(){
    printf("\nВведите команду (o x y - открыть, f x y - флаг, q - выйти): ");
}